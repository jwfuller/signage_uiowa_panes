<?php
/**
 * @file
 * Ctools content type plugin file for The University of Iowa nameplate.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Weather Forecast'),
  'description' => t('Weather Forecast data for campus.'),
  'category' => t('UIowa'),
  'edit form' => 'uiweather_edit_form',
  'render callback' => 'uiweather_render',
  'admin info' => 'uiweather_admin_info',
);


/**
 * The 'admin info' callback for panel pane.
 */
function uiweather_admin_info($subtype, $conf, $contexts) {
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
function uiweather_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function uiweather_edit_form_submit($form, &$form_state) {
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
function uiweather_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();

  $block->title = t('Weather');
  $block->content = array(
    '#prefix' => '<div id="forecastdata">',
    '#markup' => '<canvas id="skycon" width="200" height="200"></canvas><div id="temp"></div><div id="feelslike"></div><div id="currently"></div><div id="summary"></div>',
    '#suffix' => '</div>',
  );

  libraries_load('skycons');

  if (($library = libraries_load('skycons')) && !empty($library['loaded'])) {
    // if skycons library is loaded, also load in the weather javascript
    drupal_add_js(drupal_get_path('module', 'signage_uiowa_panes') . '/js/weather.js');
  }

  return $block;
}
