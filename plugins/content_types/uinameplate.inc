<?php
/**
 * @file
 * Ctools content type plugin file for The University of Iowa nameplate.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('UI nameplate'),
  'description' => t('The University of Iowa nameplate pane.'),
  'category' => t('UIowa'),
  'edit form' => 'uinameplate_edit_form',
  'render callback' => 'uinameplate_render',
  'admin info' => 'uinameplate_admin_info',
);


/**
 * The 'admin info' callback for panel pane.
 */
function uinameplate_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('The University of Iowa nameplate.');
    return $block;
  }
}

/**
 * The 'Edit form' callback for the content type.
 */
function uinameplate_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function uinameplate_edit_form_submit($form, &$form_state) {
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
function uinameplate_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();

  $block->title = t('The University of Iowa');
  $block->content = array(
    '#theme' => 'image',
    '#path' => drupal_get_path('module', 'signage_uiowa_panes') . '/uiowa_nameplate_gold.svg',
    '#alt' => 'The University of Iowa',
    '#attributes' => array(
      'class' => array(
        'uinameplate',
      ),
    ),
  );
  return $block;
}
