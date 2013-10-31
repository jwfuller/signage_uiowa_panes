<?php
/**
 * @file
 * Ctools content type plugin file for a Pentacrest Webcam pane.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Pentacrest Webcam'),
  'description' => t('Shows an image stream overlooking the Pentacrest.'),
  'category' => t('UIowa'),
  'edit form' => 'pentacrestcam_edit_form',
  'render callback' => 'pentacrestcam_render',
  'admin info' => 'pentacrestcam_admin_info',
);

/**
 * The 'admin info' callback for panel pane.
 */
function pentacrestcam_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('The University of Iowa Pentacrest Webcam.');
    return $block;
  }
}

/**
 * The 'Edit form' callback for the content type.
 */
function pentacrestcam_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function pentacrestcam_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type).
 *
 * See ctools_plugin_examples for more advanced info.
 */
function pentacrestcam_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();

  $block->title = t('Live view of the University of Iowa Pentacrest');
  $block->content = '
    <div class="webcam">
      <img class="webCamImage" src="http://webcam.iowa.uiowa.edu/live/readImage.asp" height="576" width="1024" alt="Pentacrest Cam" />
    </div>';
  drupal_add_js(drupal_get_path('module', 'signage_uiowa_panes') . '/js/pentacrestcam.js');
  return $block;
}
