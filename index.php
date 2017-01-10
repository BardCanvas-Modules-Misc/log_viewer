<?php
 /**
 * Logs viewer index
 *
 * @package    BardCanvas
 * @subpackage log_viewer
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * @param $_REQUEST["group"] Path for logs group
 * @param $_REQUEST["file"]  File name to show
 */

include "../config.php";
include "../includes/bootstrap.inc";

if( ! $account->_exists ) throw_fake_401();
$_engine_template = $settings->get("engine.template");

/** @var array $logfiles [container_path][file_key] := filename */
$logfiles = array();

$existing_logs = glob( ROOTPATH . "/logs/*.log" );
foreach ($existing_logs as $this_log)
{
    if (filesize($this_log) == 0) continue;
    
    $file_key = ucwords(str_replace(
        array(".log", "_", "-"),
        array("", " ", " "),
        basename($this_log)
    ));
    $file_key = preg_replace('/(\d+) (\d+) (\d+)/', '$1$2$3', $file_key);
    $logfiles["{$current_module->language->local_logs}"][$file_key] = $this_log;
}

$errors = array();

$current_module->load_extensions("logs_browser", "logfiles_var");

$template->page_contents_include = "contents/index.tpl.inc";
include "{$template->abspath}/admin.php";
