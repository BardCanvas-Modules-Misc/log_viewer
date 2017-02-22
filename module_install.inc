<?php
/**
 * Module installation file
 *
 * @package    BardCanvas
 * @subpackage categories
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var string $module_install_action install, uninstall, enable, disable
 * @var array  $messages
 * @var array  $errors
 * @var module $this_module
 * @var module $current_module
 */

use hng2_base\module;

if( $module_install_action == "install" )
{
    $settings->set("modules:log_viewer.for_admins_only", "true");
    
    return;
}

if( $module_install_action == "uninstall" )
{
    $settings->delete("modules:log_viewer.for_admins_only");
    
    return;
}
