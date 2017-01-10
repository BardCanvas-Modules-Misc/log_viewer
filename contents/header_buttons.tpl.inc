<?php
/**
 * Logs viewer header buttons
 *
 * @package    BardCanvas
 * @subpackage log_viewer
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

if ( ! $account->_exists) return;

if( $settings->get("modules:log_viewer.for_admins_only") == "true" ) return;

$template->add_menu_item(
    $this_module->language->menu_caption,
    sprintf(
        '
        <a class="main_menu_item %s" href="%s/%s/?wasuuup=%s">
            <span class="fa %s"></span> %s
        </a>
        ',
        $current_module->name == $this_module->name ? "current" : "",
        $config->full_root_path,
        $this_module->name,
        md5(mt_rand(1, 65535)),
        "fa-file-text-o",
        $this_module->language->menu_caption
    )
);
