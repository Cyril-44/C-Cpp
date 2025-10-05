longtask = function(selector,vpath) {
    this.selector = selector;
    this.vpath = vpath;

    //private members
    this.div = null;
    this.prev_state_update_time  = 0;

    this.l10n = { //localizable strings here
	operation_completed_successfully: 'Operation completed successfully',
	operation_failed_with_N_errors: 'Operation failed with %s error(s)',
	message_operation_in_progress:  'Please wait, operation is in progress..',
	message_operation_in_progress_with_log:  'Please wait, operation is in progress:',
	message_log_is_here: 'Here is a log of the operation:'
    };
    this.messagediv = null;
}

longtask.prototype.callback_on_finished =  window.longtask_hooks['callback_on_finished'] ||  function(this_,state) { 
};


longtask.prototype._make_msg_div = function(msg,_class) {
    this.messagediv = $("<div>", { "class": " longtask-div-" + _class , 'text': msg});    
    return this.messagediv; //ui text loader
}

longtask.prototype._setup = function() {
    $(this.selector).empty().append(	
	this._make_msg_div(this.l10n.message_operation_in_progress, 'please-wait')
    );    
}

longtask.prototype._clear_dynstatus_intervalID = function() {
    if (this.dynstatus_intervalID) {
	clearInterval(this.dynstatus_intervalID);
	this.dynstatus_intervalID = void 0;
    }
}

longtask.prototype._check_dynstatus_file = function() {
    var fn = this.last_state.file_with_cur_status;
    var this_ = this;
    if (fn.length) {
	    $.ajax({ 
		url: this.vpath + '/' + fn + '?x=' + Math.random().toString(),
		success: function(status){
		    var pos = status.indexOf('LONGTASK_STATUS_END');
		    //console.log('in longtask.prototype._check_dynstatus_file status is ' + status);		
		    if (pos != -1) { //the status is complete
			status = status.substring(0,pos);
			var diag = this_.last_state.log_diagnostics;
			//console.log("will append " + status);
			if (!diag.length) return; //log is empty, do nothing
			var when = Math.floor((new Date()).getTime()/1000);
			diag[diag.length-1] = [ status, when, status ];
			//console.log("now calling on_state_update ");
			this_._on_state_update_noticed(this_.last_state, 0);
		    }
		}
	    });
	
    };
}


longtask.prototype._on_state_update_noticed = function(new_state, check_dynstatus_file) {
    //console.log(new_state);
    this.last_state = new_state;
    var this_ = this;
    if (check_dynstatus_file) {
	if (this.last_state.file_with_cur_status.length) {
	    if (!this.dynstatus_intervalID) {
		this.dynstatus_intervalID = setInterval( function() { this_._check_dynstatus_file(); }, 1000);
	    }; 
	} else {
	    this._clear_dynstatus_intervalID();
	} 
    };

    if (new_state.status == 'starting') return; //nothing to do
    
    var elts = $('<div>', { 'class': 'longtask-tracelog' });



    if (1)
    {//regenerate html for the log
	$.each( new_state.log_diagnostics, function (idx,diag) {
		this_.decorator_for_diagnostics( elts,	$('<div>', {'class': 'longtask-tracelog-item' } ), diag, idx );
	    });
    }
    var elts2 = $('<div>', { 'class': 'longtask-after-tracelog' });
    if (new_state.status == 'finished') {
	clearInterval(this.reload_state_intervalID);
	if (!new_state.error_count) {
	    elts2.append( $('<div>', 
		{ 'class': 'longtask-status_success' }
	    ).html( this.l10n.operation_completed_successfully ) );
	} else {
	    var str = this.l10n.operation_failed_with_N_errors;

	    elts2.append( $('<div>', 
		{ 'class': 'longtask-status_failed' }
	    ).html( str.replace(/%s/g, new_state.error_count) ) ) ;

	    
	    $.each( Array.isArray(new_state.error_data) ? new_state.error_data : [ new_state.error_data ] , function(key,item) {

		this_.decorator_for_error_item(elts2, $('<div>', { 'class': 'longtask-item-error' } ),
			item, key ) ;	



	    });
	};
    };	

    var message_str = new_state.status == 'finished' ? 
		this.l10n.message_log_is_here : this.l10n.message_operation_in_progress_with_log , 
	    message_class = new_state.status == 'finished' ?  'log-is-here' : 'please-wait';
    if (new_state.status == 'finished')
    {
	var d = new_state.postscriptum_notes;
	if (d != undefined)
	    this.decorator_for_postscriptum_notes(elts2, d);
    }
    $(this.selector).empty().append(
	     this._make_msg_div(message_str, message_class)
	).append(elts);

    if (new_state.status == 'finished')
    {
        this._clear_dynstatus_intervalID();
	$(this.selector).append(elts2);
        this.callback_on_finished(this,new_state);
    }
    if (this_.should_activate_scripts())
	this_.activate_scripts( $(this_.selector).get(0) );
    //console.log(elts.html());
}



longtask.prototype.activate_scripts = function(node_) {
    /***************/
    /* from here: http://stackoverflow.com/a/20584396 */
    function nodeScriptReplace(node) {
        if ( nodeScriptIs(node) === true ) {
                node.parentNode.replaceChild( nodeScriptClone(node) , node );
        }
        else {
                var i        = 0;
                var children = node.childNodes;
                while ( i < children.length ) {
                        nodeScriptReplace( children[i++] );
                }
        }

        return node;
    }
    function nodeScriptIs(node) {
        return node.tagName === 'SCRIPT';
    }
    function nodeScriptClone(node){
        var script  = document.createElement("script");
        script.text = node.innerHTML;
        for( var i = node.attributes.length-1; i >= 0; i-- ) {
                script.setAttribute( node.attributes[i].name, node.attributes[i].value );
        }
        return script;
    }

    nodeScriptReplace(node_);
    /***************/
}

longtask.prototype.setup_and_run = function() {
    this._setup();
    var task = this;
    var iter_idx = 0;
    this.reload_state_intervalID = setInterval(function() { 
	    //console.log("requesting last-updated-time.txt " + Math.floor((new Date()).getTime()/1000) );
	    $.ajax({ 
		url: task.vpath + '/' + 'last-updated-time.txt' + '?x=' + Math.random().toString(),
		success: function(result){
		    var update_time = result;    
		    //console.log("last-updated-time.txt arrvied=  " + result);
		    if (task.prev_state_update_time != update_time) {
			task.prev_state_update_time = update_time;
			$.ajax( {
			    url: task.vpath + '/' + 'state-for-clientside.js' + '?x=' + Math.random().toString(),
			    dataType: "json",
			    success: function(new_state) { task._on_state_update_noticed(new_state, 1); }, 
			});
		    }
		}
	    });
	} , 1000);
}

longtask.prototype.get_fake_data = function() {
    return {
	    'status': 'finished',
	    'log_diagnostics': [
		    ['diag line 1',1],
		    ['diag line 2',2],
		    ['diag line 3',3]
		],
	    'error_count': 0, 
	    'error_data': null
	};
}

//////////////////these are for testing in various states
longtask.prototype.setup_and_fake_run_with_success = function(selector) {
    this._setup(selector);
    this._on_state_update_noticed(
	this.get_fake_data()
    );
};


longtask.prototype.setup_and_fake_run_with_failure = function(selector) {
    this._setup(selector);
    var d = this.get_fake_data();
    d.error_count = 3;
    d.error_data = [
	'First error message' , 'Second error message' , 'Third error message'
	];
    this._on_state_update_noticed(d);
};

longtask.prototype.setup_and_fake_run_in_progress = function(selector) {
    this._setup(selector);
    var d = this.get_fake_data();
    d.status = 'running';
    this._on_state_update_noticed(d);
};


////////////////////overridables follow
longtask.prototype.decorator_for_error_item =  window.longtask_hooks['decorator_for_error_item'] ||
    function(hubelt, element, item, key)  { 
	hubelt.append( element.text(item) ); 
}

//an array like [ string, timeend] is passed to it, for rendering diagnostic item
longtask.prototype.decorator_for_diagnostics = window.longtask_hooks['decorator_for_diagnostics'] ||
    function(hubelt, element, item) { 
	    hubelt.append(  element.text(item[0])  ); 
};

longtask.prototype.decorator_for_postscriptum_notes = window.longtask_hooks['decorator_for_postscriptum_notes'] ||
    function(hubelt, item) { 
	    //console.log(item);
	    if (0) { //this strips out scripts from  item!
		hubelt.append(  item ); 
	    } else {
	        var elt = $( '<div>');
	        hubelt.append(  elt ); 
		elt.get(0).innerHTML = item;
	    }
};

longtask.prototype.should_activate_scripts =  window.longtask_hooks['should_activate_scripts'] || function(task) { return 1; }
