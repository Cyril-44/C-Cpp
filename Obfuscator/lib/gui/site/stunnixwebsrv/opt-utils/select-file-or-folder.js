/* 
Turns given control (button or input[type=submit] into browser. The initial value will
read from and will be written to input 'entryelt'. Basedir is assumed to be 'basedir'.

basedir can be empty.

entryelt's content can be empty too
*/


//these vars are used by 'add_xxx_browser()' for default values
var fsbrowser_button_title = 'Browse for..';
var fsbrowser_button_class = 'button';

//these functions are public interface.
function fsb_turn_into_dir_browser(btnelt,entryelt,basedir)
{
    _fsb_turn_into_fs_browser('dir',btnelt,entryelt,basedir)
};

function fsb_turn_into_file_browser(btnelt,entryelt,basedir)
{
    _fsb_turn_into_fs_browser('filequery',btnelt,entryelt,basedir)
};


function fsb_add_dir_browser(entryelt,basedir)
{
    _fsb_add_fs_browser('dir',entryelt,basedir);
};
function fsb_add_file_browser(entryelt,basedir)
{
    _fsb_add_fs_browser('filequery',entryelt,basedir);
}

/* PRIVATE IMPLEMENTATION FOLLOWS*/
var _fsb_os_supported = (navigator.platform == 'Win32' || 
    navigator.platform == 'MacIntel' || navigator.platform == 'MacPPC'
    || navigator.platform.indexOf('Linux') == 0
) ? 1 : 0 ;



var _fsb_btn_idx = 1;
var _fsb_btn_pfx = '_fsb_btn_';
function _fsb_add_fs_browser(type_,entryelt,basedir)
{
    if (!_fsb_os_supported) return;    
    document.write('<span class=_fsb_button><input type=button class=' + fsbrowser_button_class + ' id=' +
	_fsb_btn_pfx + _fsb_btn_idx + ' value="' + fsbrowser_button_title +
	'"></input></span>');
    btnelt = document.getElementById(_fsb_btn_pfx + _fsb_btn_idx);
    ++_fsb_btn_idx;
    _fsb_turn_into_fs_browser(type_,btnelt,entryelt,basedir);
};

function _fsb_turn_into_fs_browser(type_,btnelt,entryelt,basedir)
{
    btnelt.onclick = !_fsb_os_supported ? function () {alert(
	'Browsing for files and directories is not supported on this platform. Please type the path manually!');} 
    : function () { _fsb_button_clicked(type_,entryelt,basedir); };
}

var _fsb_target_entry;
function _fsb_button_clicked_old(type_,entryelt,basedir)
{
    if (!_fsb_os_supported) return;      
    var curval = entryelt.value;
    jx.load('/stunnix-websrv-utils/browse-files/selectfile.' +
	(navigator.platform == 'Win32' ? 'exe' : 'pl') +
	'?type=' + 
	(type_ == 'dir' ? 'dirquery' : 'filequery') + '&basedir=' + 
	escape(basedir) + '&xmlhttp=1&curval=' + escape(curval),
		function(newval){ 
		    newval = newval.replace(/[\r\n]/g,"");
		    if (newval.length) entryelt.value = newval;
		}
	);
};

function _fsb_button_clicked(type_,entryelt,basedir)
{
    if (!_fsb_os_supported) return;      
    var curval = entryelt.value;

    fsb_browser_run_ajax(type_, 
		{ //options
		'basedir': basedir,
		'curval': curval
		},
		{ //callbacks
		'onsuccess': function (newval) 	{  entryelt.value = newval; }
		}
	);
};



function fsb_browser_run_ajax_fallback(type_,options,callbacks)
{
    var hints = {'filequery' : 'full path to the file',
		'dirquery': 'full path to the directory',
		'savefile': 'full path to the file to write to'
	    };
    hints['dir'] = hints['dirquery'];
    var txt = "There is no 'zenity' tool installed on your system, and it's used to show file/directory selection boxes.\n" +
	"Please install the tool (most probably command 'apt-get install zenity') and after that press 'Cancel',\n" +
	" or type in " + hints[type_] + " and click OK";
    var ret = prompt(txt,options.curval);    
    if (ret == null) {
	fsb_browser_run_ajax(type_,options,callbacks);
	return {'retry_done':1};
    } else {
	return { 'path' : ret, 'filteridx_selected': 0 };
    };
}

/*  Sample usage:

	fsb_browser_run_ajax('filequery', //or 'dirquery' or 'savefile'
		{ //options
		'basedir':'',
		'curval':'',
		'curval':'c:\\temp\\run1.txt',
		'fileexts': "*.png\n*.jpg\n*.*", //\n-separated filters (file patterns)
		'fileexts_titles': "PNG files\nJPG files\nAll files",  //\n-separated titles for filters 
		'curfileext_idx':2, //1-based index of currently selected filter
		'autoappend_extension':'png' //for Save dialogue: pass extension to append if user types no extension and filter is *.*
			//It's greatly recommended to pass something here - e.g. extension of the 1st filter. This value is guessed
			//if not passed explicitly
		},
		{ //callbacks
		'onsuccess': function (newval) 	{  $('#fortext').html('success: ' + newval); },
		'onfail':    function (errcode)	{  $('#fortext').html('fail, code: ' + errcode); }
		'on_error_no_zenity': function (type_,options,callbacks) > { } //will be called if present. If defined, no
		    //built-in dialogue will be shown
		}
	);

*/
var fsb_server_is_win32 = navigator.platform == 'Win32' ? 1 : 0;
function fsb_browser_run_ajax(type_,options,callbacks)
{
    if (!_fsb_os_supported) {
		if (options['onfail']) options['onfail']('NOTSUPPORTED');
		return;
	};
    var options_part = '';
    for(var k in options) 
    {
	options_part += '&' + k + '=' + encodeURIComponent(options[k]);
    }
    jx.load('/stunnix-websrv-utils/browse-files/selectfile.' +
	(fsb_server_is_win32 ? 'exe' : 'pl') +
	'?type=' + type_ + '&response_type=json' + options_part,
		function(newval_obj){ 
		    var newval = newval_obj['path'], filteridx_selected = newval_obj['filteridx_selected'];
		    if (newval_obj['error'] == 'ERROR_NO_ZENITY_INSTALLED') {
			//it's linux, and we can't run zenity
			newval_obj = callbacks['on_error_no_zenity'] ? callbacks['on_error_no_zenity'](type_,options,callbacks) 
			    : fsb_browser_run_ajax_fallback(type_,options,callbacks);
			newval = newval_obj['path'];
			if (newval_obj['retry_done'])
			    return;
		    };
		    //newval = newval.replace(/[\r\n]/g,"");
		    if (newval.length) {
			if (callbacks['onsuccess']) callbacks['onsuccess'](newval,newval_obj);
		    } else {
			if (callbacks['onfail']) callbacks['onfail']('CANCELLED');
		    }
		}
	,'json');
      
}


//V3.00.A - http://www.openjs.com/scripts/jx/
jx = {
	http:false, //HTTP Object
	format:'text',
	callback:function(data){},
	error:false,
	//Create a xmlHttpRequest object - this is the constructor. 
	getHTTPObject : function() {                                    
		var http = false;
		//Use IE's ActiveX items to load the file.
		if(typeof ActiveXObject != 'undefined') {               
			try {http = new ActiveXObject("Msxml2.XMLHTTP");}
			catch (e) {
				try {http = new ActiveXObject("Microsoft.XMLHTTP");}
				catch (E) {http = false;}
			}
		//If ActiveX is not available, use the XMLHttpRequest of Firefox/Mozilla etc. to load the document.
		} else if (XMLHttpRequest) {
			try {http = new XMLHttpRequest();}
			catch (e) {http = false;}
		}
		return http;
	},
	// This function is called from the user's script. 
	//Arguments - 
	//	url	- The url of the serverside script that is to be called. Append all the arguments to 
	//			this url - eg. 'get_data.php?id=5&car=benz'
	//	callback - Function that must be called once the data is ready.
	//	format - The return type for this function. Could be 'xml','json' or 'text'. If it is json, 
	//			the string will be 'eval'ed before returning it. Default:'text'
	load : function (url,callback,format) {
		this.init(); //The XMLHttpRequest object is recreated at every call - to defeat Cache problem in IE
		if(!this.http||!url) return;
		if (this.http.overrideMimeType) this.http.overrideMimeType('text/xml');

		this.callback=callback;
		if(!format) var format = "text";//Default return type is 'text'
		this.format = format.toLowerCase();
		var ths = this;//Closure
		
		if (this.http.overrideMimeType) this.http.overrideMimeType('text/xml');

		//Kill the Cache problem in IE.
		var now = "uid=" + new Date().getTime();
		url += (url.indexOf("?")+1) ? "&" : "?";
		url += now;

		this.http.open("GET", url, true);

		this.http.onreadystatechange = function () {//Call a function when the state changes.
			if(!ths) return;
			var http = ths.http;
			if (http.readyState == 4) {//Ready State will be 4 when the document is loaded.
				if(http.status == 200) {
					var result = "";
					if(http.responseText) result = http.responseText;
					//If the return is in JSON format, eval the result before returning it.
					if(ths.format.charAt(0) == "j") {
						//\n's in JSON string, when evaluated will create errors in IE
						result = result.replace(/[\n\r]/g,"");
						result = eval('('+result+')'); 
					}
	
					//Give the data to the callback function.
					if(ths.callback) ths.callback(result);
				} else { //An error occured
					if(ths.error) ths.error()
				}
			}
		}
		this.http.send(null);
	},
	init : function() {this.http = this.getHTTPObject();}
}

jx.load('/webserver-special-interface/status-all-json.js',function(obj){ 
	fsb_server_is_win32 = (obj['platform'] == 'win') ? 1: 0;
    } , 'json');
