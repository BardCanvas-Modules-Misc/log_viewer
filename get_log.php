<?php
/**
 * Log retriever
 *
 * @package    BardCanvas
 * @subpackage log_viewer
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_tools\cli;

include "../config.php";
include "../includes/bootstrap.inc";

if( ! $account->_exists ) throw_fake_401();

header("Content-Type: text/html; charset=utf-8");

if( empty($_REQUEST["logfile"]) ) die($current_module->language->no_log_provided);

if( substr($_REQUEST["logfile"], 0, 11) == "internal://" )
{
    $absoulte_root_url   = "{$config->full_root_url}/";
    $_REQUEST["logfile"] = str_replace("internal://", $absoulte_root_url, $_REQUEST["logfile"]);
    $passhash            = md5($config->encryption_key1);
    
    if(stristr($_REQUEST["logfile"], "?") !== false)
        $_REQUEST["logfile"] .= "&passhash=" . urlencode($passhash);
    else
        $_REQUEST["logfile"] .= "?passhash=" . urlencode($passhash);
    
    $_REQUEST["logfile"] .= "&offset="   . $_REQUEST["offset"];
    
    $contents = file_get_contents($_REQUEST["logfile"]);
    die($contents);
}

$log = "{$config->logfiles_location}/{$_REQUEST["logfile"]}";
if( ! file_exists($log) ) die($current_module->language->invalid_log_provided);

$contents = file_get_contents($log);
$html = cli::to_html($contents);
echo $html;
