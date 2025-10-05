if (navigator.appName.indexOf('Netscape')!=-1 || 
    navigator.appName.indexOf('Konqueror')!=-1 )
{
    document.write('<style type=text/css src=/res/mozres.css></style>');

}
document.write('<style type=text/css>div.longdesc { display: none; }</style>');
if (0)
{
    var alltrans = [
	    "Fade(Duration=1.0)"
	    ,"RandomDissolve(Duration=1.0)"
	    //,"Stretch(Duration=1.0,StretchStyle='PUSH')"
    ];
    var t = alltrans[Math.floor(Math.random()*(alltrans.length))];
    document.write('<meta http-equiv="Page-Exit" content="progid:DXImageTransform.Microsoft.'+t+'">');
    document.write('<meta http-equiv="Site-Enter" content="progid:DXImageTransform.Microsoft.'+t+'">');
    document.write('<meta http-equiv="Site-Exit" content="progid:DXImageTransform.Microsoft.'+t+'">');
}
var curselected = '';
function ge_by_id(nm)
{
    return document.getElementById(nm);
}
function showhide(nm)
{
    if (curselected.length)
    {
	ge_by_id(curselected).style.display = 'none';
	ge_by_id(curselected + '.detailsbtn').innerHTML = "<img src=/res/btn-details.gif alt='show details' class=noborder width=10 height=10>";
    }
    if (nm != curselected)
    {
	curselected = nm;
	ge_by_id(curselected).style.display = 'block';
	ge_by_id(curselected + '.detailsbtn').innerHTML = "<img src=/res/btn-details-hide.gif alt='hide details'  class=noborder  width=10 height=10>";
	jx.load('track.html?' + nm,function() {})
    } else {
	curselected = '';
    }
}

function register_dl(e,url)
{
    urchinTracker(url);
}




/** part related to ads sliding in or out at the left of the page */


var left_ad_info = { //describes complete ad location
    allclips: { //all ads possible. If some "value" is empty string, then it's disabled
        saws: "/prod/aws/overview.shtml",
        addfeature: "/contact/",
	allobfus: "/prod/",
        anyquestions: "/contact/",
	outsource: "/contact/",
        jsobfus: "/prod/jo/overview.shtml",
        perlobfus: "/prod/po/overview.shtml",
        vbsobfus: "/prod/vbso/overview.shtml"    
    }, 
    ad_sequence: [], //defines list of keys, each key is ID of clip to show. It's filled once
	    //randomly - it determines in which order ads are shown

    ad_sliding_step_ms: 20,
    ad_unsliding_step_ms: 12, //how much time to tait before each step
    shown_duration: 10000, //how long ad is shown
    hidden_duration: 3000,

    ad_sliding_direction: 'right',    

    objId: 'floater_content',




    unslided_offs: -108, //how many pix div is currently shifted
    slided_offs: 0, //how many pix div is currently shifted
    totalsteps: 108, //width of clip that slides in the direction of sliding

    state: 'hidden', //can be 'sliding','showing',unsliding','hidden'
    cur_offs: -108,
    clip_idx: 0,

    enable_sliding: 0
}


function fisherYates ( myArray ) { //shuffles the array
  var i = myArray.length;
  if ( i == 0 ) return false;
  while ( --i ) {
     var j = Math.floor( Math.random() * ( i + 1 ) );
     var tempi = myArray[i];
     var tempj = myArray[j];
     myArray[i] = tempj;
     myArray[j] = tempi;
   }
}

function myt(s)
{
    //alert(s);
}	

function start_showing_sliding_ads(a,varnm)
{
    if (!a.ad_sequence.length)
    {	//fill it
	for(var i in a.allclips)
	{
	    if (a.allclips[i].length)
		a.ad_sequence[a.ad_sequence.length]=i; //skip empty strings
	}
	fisherYates(a.ad_sequence);
	if (!a.enable_sliding)
	{
	    var clipid = a.ad_sequence[a.clip_idx++];
	    myt('no sliding, clipid ' + clipid);
	    if (a.clip_idx==a.ad_sequence.length)
		a.clip_idx=0;
	    {
		var img  = document.getElementById(a.objId + '_img');
	        var href = document.getElementById(a.objId + '_href');
		if (img)
		    img.src = '/res/stx-ad-' + clipid + '.gif';
		if (href)
		    href.href = a.allclips[clipid] + '?sidead=' + clipid;
	    }	

	    var o = document.getElementById(a.objId);
	    var size_prop = a.ad_sliding_direction == 'right' ? 'left' : 'top'; //flextodo
	    if (o)
		o.style[size_prop] = '0px';
	    return;
	};
    }
    if (a.state =='hidden')
    {
	a.state = 'sliding';
	a.steps_made = 0;
	myt("was hidden, now sliding");

	
	var clipid = a.ad_sequence[a.clip_idx++];
	if (a.clip_idx==a.ad_sequence.length)
	    a.clip_idx=0;
	{
	    var img  = document.getElementById(a.objId + '_img');
	    var href = document.getElementById(a.objId + '_href');
	    img.src = '/res/stx-ad-' + clipid + '.gif';
	    href.href = a.allclips[clipid] + '?sidead=' + clipid;
	}	
    } else if (a.state =='showing') {
	a.state = 'unsliding';
	myt("was hidden, now unsliding");
	a.steps_made = 0;
    }
    var _sgn;
    if (a.state == 'sliding' || a.state == 'unsliding')
    {
	_sgn = a.ad_sliding_direction == 'right' ? 1  : -1;  //flextodo

    };
    if (a.state == 'unsliding')
	_sgn = - _sgn;


    /* slide 1 step */


    {
	var size_prop = a.ad_sliding_direction == 'right' ? 'left' : 'top'; //flextodo
	a.cur_offs += _sgn;
	var o = document.getElementById(a.objId);
	o.style[size_prop] = a.cur_offs + 'px';
    }    


    var cb_str = "start_showing_sliding_ads(" + varnm + ",'" + varnm + "')";
    var cb_timeout;
    if (++a.steps_made == a.totalsteps)
    {
	a.state = (a.state == 'sliding') ? 'showing' : 'hidden';
	a.cb_timeout =  a.state == 'showing' ? a.shown_duration : a.hidden_duration;
	a.steps_made = 0;
    } else {
	a.cb_timeout = a.state == 'sliding' ? a.ad_sliding_step_ms : a.ad_unsliding_step_ms;	
    };
    myt("step is " + _sgn + " new cur offs=" + a.cur_offs + " next timeout=" + a.cb_timeout +
	" next_state=" + a.state);
    if (a.enable_sliding)
	a.tmid = setTimeout(cb_str,a.cb_timeout);    
}

if (left_ad_info.enable_sliding ) {
    left_ad_info.tmid = setTimeout("start_showing_sliding_ads(left_ad_info,'left_ad_info')",
    left_ad_info.enable_sliding? 4000 : 100);
} else {
    ;
    //document.onLoad = function () { 
    //start_showing_sliding_ads(left_ad_info,'left_ad_info'); 
    //};
};

//V3.00.A - downloaded from http://www.openjs.com/scripts/jx/
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


