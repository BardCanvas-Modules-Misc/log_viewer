<?php
/**
 * Logs viewer bootstrap
 *
 * @package    BardCanvas
 * @subpackage log_viewer
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

global $account, $settings, $modules;

if( $settings->get("modules:log_viewer.for_admins_only") != "true" )
    unset( $modules["log_viewer"]->menu_items );
