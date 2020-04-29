<?php
/**
 * Log viewer helper divs
 *
 * @package    BardCanvas
 * @subpackage log_viewer
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */
?>

<!-- Log viewer helpers -->

<div id="realtime_log_viewer" title="<?= $modules["log_viewer"]->language->realtime_viewer_title ?>" style="display: none;">
    <pre class="realtime_log_output" data-log="" data-previous-length="0" 
         data-wait-caption="<br><?= $language->wait ?><br><br>"
         ><br><?= $language->wait ?><br><br></pre>
</div>
