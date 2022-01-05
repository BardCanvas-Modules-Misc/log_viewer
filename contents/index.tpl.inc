<?php
/**
 * Logs viewer index
 *
 * @package    BardCanvas
 * @subpackage log_viewer
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var module $current_module
 * @var array  $logfiles       [container_path][file_key] := filename
 * @var string $selected_log
 */

use hng2_base\module;
use hng2_tools\cli;

/** @var array $subgrouped_logs [$log_group][$subgroup_name][$log_key] => $logfile_path */
$subgrouped_logs = array();

foreach($logfiles as $log_group => $logs_in_group)
{
    foreach($logs_in_group as $log_key => $this_logfile)
    {
        $log_key = preg_replace('/([\d]{4}) +([\d]{2}) +([\d]{2})/', '\1\2\3', $log_key);
        # $log_key = preg_replace('/([\d]{8}) +([\d]{6})/', '\1-\2', $log_key);
        $parts = explode(" ", $log_key);
        if( is_numeric(end($parts)) )
        {
            array_pop($parts);
            if( is_numeric(end($parts)) ) array_pop($parts);
            $subgroup_name = implode(" ", $parts);
        }
        else
        {
            $subgroup_name = "ZZZZZZZZZZZZZZZZZZ";
        }
        $subgroup_name = preg_replace('/\b(log|logs)\b/i', "", $subgroup_name);
        $subgroup_name = trim($subgroup_name);
        
        $subgrouped_logs[$log_group][$subgroup_name][$log_key] = $this_logfile;
        ksort($subgrouped_logs[$log_group]);
    }
}

/** @var array $custom_views ["Link_caption"] => "extension_handler" */
$custom_views = array();
$page_title   = $current_module->language->index_title;
$current_module->load_extensions("logs_browser", "pre_rendering");

$selected_log = $logfiles[$_GET["group"]][$_GET["file"]];
?>
<style type="text/css">
    .log_picker select { max-width: 150px; }
    .log_output        { border: 2px inset silver; border-radius: 5px; }
    .group_separator   { display: inline-block; font-weight: bold; clear: both; }
</style>

<h1 class="clearfix">
    <?
    $url = "{$_SERVER["PHP_SELF"]}"
        . "?group="       . urlencode(stripslashes($_REQUEST["group"])) 
        . "&file="        . urlencode(stripslashes($_REQUEST["file"]))
        . "&custom_view=" . urlencode(stripslashes($_GET["custom_view"]))
        . "&wasuuup="     . md5(mt_rand(1, 65535))
    ;
    ?>
    <button id="refresh_current_log" class="pull-right" onclick="location.href = '<?= $url ?>'">
        <span class="fa fa-refresh"></span>
        <?= $language->refresh ?>
    </button>
    <?
    echo $page_title;
    if( ! empty($_REQUEST["file"]) ):
        echo ": " . $_REQUEST["group"] . "/" . $_REQUEST["file"];
        
        if( ! empty($selected_log) && is_file($selected_log) ):
            $selected_logrpath = str_replace("{$config->logfiles_location}/", "", $selected_log);
            ?>
            <button onclick="$('#realtime_log_viewer').data('on-close-exec-function', function() { $('#refresh_current_log').click(); }); launch_realtime_log_viewer('<?= $selected_logrpath ?>')">
                <i class="fa fa-window-maximize"></i>
                <?= $current_module->language->live_view ?>
            </button>
            <?
        endif;
    endif;
    ?>
</h1>

<? if ( ! empty($errors)): ?>
    <div class="framed_content state_ko">
        <? foreach($errors as $error): ?>
            <span class="fa fa-warning"></span>
            <?= $error ?><br>
        <? endforeach; ?>
    </div>
<? endif; ?>

<? if (empty($logfiles)): ?>
    <div class="framed_content state_warning">
        <span class="fa fa-info-circle"></span>
        <?= $current_module->language->messages->no_logs_to_show ?>
    </div>
    <? return; ?>
<? endif; ?>

<div class="log_picker framed_content">
    <? foreach($subgrouped_logs as $log_group => $subgroups): ?>
        
        <? if( count($subgrouped_logs) > 1 ): ?>
            <span class="group_separator"><?=ucwords(str_replace("_", " ", $log_group))?>:</span>
        <? endif; ?>
        
        <? foreach($subgroups as $subgroup_name => $logs_in_group):
            if( $subgroup_name == "ZZZZZZZZZZZZZZZZZZ" ) $subgroup_name = trim($current_module->language->other);
            $called_log_is_here = isset($logs_in_group[$_REQUEST["file"]]); ?>
            
            <select onchange="location.href = this.options[this.selectedIndex].value" style="margin-bottom: 5px; <? if($called_log_is_here) echo "background-color: Highlight; color: HighlightText;"; ?>">
                
                <option disabled <? if( ! $called_log_is_here ) echo "selected"; ?> style="background-color: Window; color: ButtonShadow;"><?= $subgroup_name ?></option>
                
                <?
                arsort($logs_in_group);
                $rendered = 0;
                foreach($logs_in_group as $log_key => $this_logfile):
                    if( $log_key == $_REQUEST["file"] )
                        $style = "background-color: Highlight; color: HighlightText;";
                    elseif (stristr($log_key, date("Ymd")) !== false)
                        $style = "background-color: palegreen; color: black;";
                    else
                        $style = "background-color: Window; color: WindowText;"
                    ?>
                    
                    <option <? if($log_key == $_REQUEST["file"]) echo "selected"; ?>
                            value="<?=$_SERVER["PHP_SELF"]?>?group=<?=urlencode($log_group)?>&file=<?=urlencode($log_key)?>&wasuuup=<?=md5(mt_rand(1, 65535))?>"
                            style="<?= $style ?>">
                        <?= $log_key ?>
                        <? if( file_exists($this_logfile) ) echo sprintf("[%s KiB]", number_format(filesize($this_logfile) / 1024, 2)); ?>
                    </option>
                    
                    <?
                    $rendered++;
                    
                    if( $rendered > 100 ) break;
                endforeach;
                ?>
                
            </select>
            
        <? endforeach; ?>
        
    <? endforeach; ?>
    
    <? if( count($subgrouped_logs) > 1 && ! empty($custom_views) ): ?>
        <span class="group_separator"><?= $current_module->language->other ?>:</span>
    <? endif; ?>
    
    <? foreach($custom_views as $caption => $key):
        $class = $_GET["custom_view"] == $key ? "state_active" : ""; ?>
        <a class="framed_content <?= $class ?> inlined" style="text-decoration: none;"
           href="<?= $_SERVER["PHP_SELF"] ?>?custom_view=<?= $key ?>&wasuuup=<?= md5(mt_rand(1, 65535)) ?>"><?= $caption ?></a>
    <? endforeach; ?>
    
</div>

<?
$current_module->load_extensions("logs_browser", "custom_view");
if( ! empty($_GET["custom_view"]) ) return;
?>

<? if( empty($_GET["group"]) && empty($_GET["file"]) ) : ?>
    
    <div class="framed_content state_ok" style="padding-top: 20px; padding-bottom: 20px;">
        <span class="fa fa-info-circle"></span>
        <?= $current_module->language->messages->select_a_log ?>
    </div>
    
    <?
    return;
endif;

if( ! is_file($selected_log) )
    die("
        <div class=\"framed_content state_ko\">
            {$current_module->language->invalid_log_provided}
        </div>
    ");

$contents = htmlspecialchars(file_get_contents($selected_log));
$contents = cli::to_html($contents);
?>
<pre class="log_output"><?= str_replace("<br>", "", $contents) ?></pre>
