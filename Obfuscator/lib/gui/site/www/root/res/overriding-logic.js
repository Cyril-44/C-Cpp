var idx = 0;
$(function() {    
    $(".overriden-content  input, .overriden-content  select,.overriden-content  .label-for-setting," + 
	" .overriden-enclosing-for-alternatives input, .overriden-enclosing-for-alternatives select, " + 
	".overriden-enclosing-for-alternatives .label-for-setting" ).each(function( index ) 
	{ $(this).wrap('<span></span>'); });

    

    $("input.overrider-for-single-setting" ).each(function( index ) { update_dependant_settings( $(this));     });
    $("input.overrider-for-single-setting" ).on('change', function() {	update_dependant_settings(this);    	});

    $("input.overrider-for-alternatives").each(function( index ) {    update_dependant_settings( $(this));     });
    $("input.overrider-for-alternatives" ).on('change', function() {	update_dependant_settings(this);       });
});


var inside_all_set_state_inside_span = 0;
function update_dependant_settings(n)
{

    var is_checked = $(n).prop('checked');
    all_set_state_inside_span($(n).prop('name') + '_enclosing', is_checked, $(n).prop('name') );
    all_set_state_inside_span($(n).prop('name') + '_enclosing_variants', is_checked, $(n).prop('name') ); //this one is for alternatives only
}

function all_set_state_inside_span(spanid,state,master_checkbox_id)
{
    var $container = $("#" + spanid);
    if (!$container) return;

    /* prevent stack overflow */
    if (inside_all_set_state_inside_span > 5) return;
    ++inside_all_set_state_inside_span;

    var sel = "input, select, .label-for-setting";
    var prop_disabled = 'disabled'
    if (state) {
	$container.find(sel).removeClass('disabled-element').prop('title','').parent().unbind('mouseover mouseout mouseenter mouseleave');
	$container.find("input, select").prop(prop_disabled,false);
	
	$container.find("input.overrider-for-single-setting" ).each(function( index ) { update_dependant_settings( $(this));   });
    } else {
	$container.find(sel).parent().unbind('mouseover mouseout mouseenter mouseleave');

	$container.find(sel).addClass('disabled-element').prop('title','Check highlighted checkbox to the left to allow editing this setting (by overriding the value set in parent mode)')
	    .parent().on('mouseenter',function() {
		    var e = $('#' + master_checkbox_id).parent();
		    e.prop('bgcolor-was', e.css('background-color'));
		    e.css({"background-color":'#f88'});
		}).on('mouseleave',function() {
		    var e = $('#' + master_checkbox_id).parent();		    
		    e.css({"background-color":e.prop('bgcolor-was')});
		}
	    );
	$container.find("input, select").prop(prop_disabled,true);
    }
    --inside_all_set_state_inside_span;
}

/* we have to undisable all forms, otherwise they won't be submitted at all! this is what W3C spec says */
function on_mode_props_submitted()
{
    $("input, select").prop('disabled',false);
}
