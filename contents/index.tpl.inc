<?php
/**
 * Logs viewer index
 *
 * @package    BardCanvas
 * @subpackage log_viewer
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * @var array  $logfiles       [container_path][file_key] := filename
 * @var string $selected_log
 */

use hng2_tools\cli;
?>
<style type="text/css">
    .log_picker select { max-width: 150px; }
    .log_output { background-color: black; color: silver; font-family: 'Lucida Console', 'Courier new', Courier, monospace;
                  font-size: 9pt; line-height: 5pt; padding: 10px; border: 2px inset silver; border-radius: 10px; }
    .group_separator { display: inline-block; font-weight: bold; }
</style>

<h1 class="clearfix">
    <a class="pull-right" href="<?=$_SERVER["PHP_SELF"]?>?group=<?=urlencode(stripslashes($_REQUEST["group"]))?>&file=<?=urlencode(stripslashes($_REQUEST["file"]))?>&wasuuup=<?=md5(mt_rand(1, 65535))?>">
        <span class="fa fa-refresh"></span>
        <?= $language->refresh ?>
    </a>
    <?
    echo $current_module->language->index_title;
    if( ! empty($_REQUEST["file"]) ) echo ": " . $_REQUEST["group"] . "/" . $_REQUEST["file"];
    ?>
    <br clear="all">
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

<?
$subgrouped_logs = array();
foreach($logfiles as $log_group => $logs_in_group)
{
    foreach($logs_in_group as $log_key => $this_logfile)
    {
        $log_key = preg_replace('/([\d]{4}) +([\d]{2}) +([\d]{2})/', '\1\2\3', $log_key);
        $log_key = preg_replace('/([\d]{8}) +([\d]{6})/', '\1-\2', $log_key);
        $parts = explode(" ", $log_key);
        if( is_numeric(end($parts)) )
        {
            array_pop($parts);
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
?>

<div class="log_picker framed_content">
    <? foreach($subgrouped_logs as $log_group => $subgroups): ?>
        
        <span class="group_separator"><?=ucwords(str_replace("_", " ", $log_group))?>:</span>
        
        <? foreach($subgroups as $subgroup_name => $logs_in_group):
            if( $subgroup_name == "ZZZZZZZZZZZZZZZZZZ" ) $subgroup_name = "Others";
            $called_log_is_here = isset($logs_in_group[$_REQUEST["file"]]); ?>
            
            <select onchange="location.href = this.options[this.selectedIndex].value" style="margin-bottom: 5px; <? if($called_log_is_here) echo "background-color: Highlight; color: HighlightText;"; ?>">
                
                <option disabled <? if( ! $called_log_is_here ) echo "selected"; ?> style="background-color: Window; color: ButtonShadow;"><?= $subgroup_name ?></option>
                
                <? arsort($logs_in_group); ?>
                <? foreach($logs_in_group as $log_key => $this_logfile):
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
                        [<?= number_format(filesize($this_logfile) / 1024, 2) ?> KiB]
                    </option>
                
                <? endforeach; ?>
            
            </select>
        
        <? endforeach; ?>
        
        <br>
    
    <? endforeach; ?>
</div>

<? if( empty($_GET["group"]) && empty($_GET["file"]) ) : ?>
    
    <div class="framed_content state_ok" style="padding-top: 20px; padding-bottom: 20px;">
        <span class="fa fa-info-circle"></span>
        <?= $current_module->language->messages->select_a_log ?>
    </div>
    
    <?
    return;
endif;

$selected_log = $logfiles[$_GET["group"]][$_GET["file"]];
$contents     = file_get_contents($selected_log);
$contents     = cli::to_html($contents);

?>
<pre class="log_output"><?=$contents?></pre>
