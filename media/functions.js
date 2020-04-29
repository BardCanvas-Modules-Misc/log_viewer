
var realtime_log_fetching_interval  = null;
var realtime_log_fetching_getting   = false;
var realtime_log_is_incremental     = false;
var realtime_log_fetching_heartbeat = 3000;

//noinspection JSUnusedGlobalSymbols
function launch_realtime_log_viewer(log, is_incremental)
{
    realtime_log_is_incremental = !!is_incremental;
    
    var $dialog = $('#realtime_log_viewer');
    
    if( realtime_log_fetching_interval ) clearTimeout(realtime_log_fetching_interval);
    realtime_log_fetching_interval = null;
    realtime_log_fetching_getting  = false;
    
    $dialog.find('.realtime_log_output').attr('data-log',             log);
    $dialog.find('.realtime_log_output').attr('data-previous-length', 0  );
    
    var wait_caption = $dialog.find('.realtime_log_output').attr('data-wait-caption');
    $dialog.find('.realtime_log_output').html(wait_caption);
    
    $dialog.dialog({
        width:  $(window).width()  - 100,
        height: $(window).height() - 100,
        close:  function()
        {
            if( realtime_log_fetching_interval ) clearTimeout(realtime_log_fetching_interval);
            $(this).dialog('destroy');
            
            if( typeof $(this).data('on-close-exec-function') == 'function' )
            {
                var fn = $(this).data('on-close-exec-function');
                if( fn ) fn();
            }
            
            if( $(this).data('on-close-redirect-url') )
            {
                stop_notifications_getter();
                location.href = $(this).data('on-close-redirect-url');
            }
        }
    });
    
    //realtime_log_fetching_interval = setTimeout("update_realtime_log_fetching()", realtime_log_fetching_heartbeat);
    update_realtime_log_fetching();
}

function update_realtime_log_fetching()
{
    if( realtime_log_fetching_getting ) return;
    
    realtime_log_fetching_getting = true;
    
    var $dialog         = $('#realtime_log_viewer');
    var $container      = $dialog.find('.realtime_log_output');
    var log             = $container.attr('data-log');
    var previous_length = parseInt($container.attr('data-previous-length'));
    
    var url = $_FULL_ROOT_PATH
            + '/log_viewer/get_log.php'
            + '?logfile='  + encodeURIComponent(log)
            + '&offset='   + previous_length
            + '&wasuuup='  + parseInt(Math.random() * 1000000000000000)
            ;
    
    $.get(url, function(response)
    {
        if( ! realtime_log_is_incremental ) $container.html(response);
        else                                $container.append(response);
        
        var new_length = response.length;
        
        var storing_length = realtime_log_is_incremental
            ? previous_length + new_length
            : new_length;
        
        $container.attr('data-previous-length', storing_length);
        
        realtime_log_fetching_getting = false;
        
        // console.log('Realtime Log Viewer> previous length: ', previous_length, ' / incoming data length: ', new_length , ' / new length: ', storing_length);
        if( previous_length !== storing_length ) $dialog.scrollTo('max', 'fast', {axis: 'y'});

        realtime_log_fetching_interval = setTimeout("update_realtime_log_fetching()", realtime_log_fetching_heartbeat);
    });
}

function set_update_realtime_log_close_redirect(url)
{
    $('#realtime_log_viewer').data('on-close-redirect-url', url);
}
