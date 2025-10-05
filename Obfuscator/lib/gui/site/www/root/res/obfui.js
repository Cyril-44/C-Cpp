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



/* show/hide functionality*/
var _show_hide_idx = 0;
var _show_hide_state = [];
var _show_hide_str_show = "&nbsp;&nbsp;&#9660;&nbsp;&nbsp;";
var _show_hide_str_hide = "&nbsp;&nbsp;&#9650;&nbsp;&nbsp;";
var _show_hide_header_height = "1.8em";
function showHide_setup_parent_div()
{

    var is_visible = 0;
    _show_hide_state[_show_hide_idx] = is_visible;

    {
        var str = "<span class=showhide id=_showhide_span_" + _show_hide_idx +
            " onclick='_show_hide_toggle(" +
            _show_hide_idx +    ")'>" +
        (is_visible ? _show_hide_str_hide : _show_hide_str_show) +
        '</span>';
        document.write(str);
    }

    var thediv;
    {
        var spanid = "_showhide_span_"  + _show_hide_idx;
        var elem = document.getElementById(spanid);
        while (elem.tagName != 'DIV')
            elem = elem.parentNode;
        thediv = elem;
    }
    /* now we have 'thediv' that encloses block to be shown or hidden */
    {
        thediv.id = "_show_hide_div_" + _show_hide_idx;
        thediv.style.height = is_visible ? 'auto' :
            _show_hide_header_height;
        thediv.style.overflow = 'hidden';
    }
    ++_show_hide_idx;
}

function _show_hide_toggle(idx)
{
    var thediv = document.getElementById("_show_hide_div_" + idx);
    if (!thediv)
        return alert("no div found " + idx);
    var oldstate = _show_hide_state[idx];
    var newstate = oldstate ? 0 : 1;

    {
        var span = document.getElementById('_showhide_span_' + idx);
        span.innerHTML = newstate ? _show_hide_str_hide :
            _show_hide_str_show;
    }
    {
        thediv.style.height = newstate ? 'auto' : _show_hide_header_height;
        _show_hide_state[idx] = newstate;
    }
}


var modesample_curid = "";
function modesample_clicked(id)
{
    var newobj = ge_by_id(id);    
    if (typeof modesample_curid === "undefined")
        modesample_curid = "";
    if (modesample_curid == id)
	return;
    if (modesample_curid.length)
    {
	var oldobj = ge_by_id(modesample_curid);
	oldobj.className = "code";
    }
    modesample_curid = id;
    newobj.className = 'code code-selected';

    ge_by_id('newbase').value = id;
}
