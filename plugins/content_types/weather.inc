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
  'defaults' => array(
    'three_day' => 0,
    'current' => 1,
    '24_hour' => 0,
  ),
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
  //Create checkbox to toggle three day forecast
  $form['three_day'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Three day forecast'),
    '#description' => t('Check this box to display three day forecast data'),
    '#default_value' => $conf['three_day'],
  );
  //Create checkbox to toggle current weather data
  $form['current'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the current weather'),
    '#description' => t('Check this to display the current weather data'),
    '#default_value' => $conf['current'],
  );
  //Create checkbox to toggle the next 24 hour prediction
  $form['24_hour'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show next 24 hour prediction'),
    '#description' => t('Check this to display the prediction for the next 24 hours'),
    '#default_value' => $conf['24_hour'],
  );
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
  //Declare variables to hold markup for each configuration option
  $three_day_content = '';
  $current_content = '';
  $twenty_four_content = '';
//If the three day forecast should be shown, add its markup to the three day content
  if ($conf['three_day'] == 1) {
    $three_day_content = "<div id='three-day-forecast'> <div id='today'> <div id='today-day'></div> <canvas id='today-skycon' width ='50' height='50'></canvas> <div class='temperature-container'> <div id='today-max'></div> <div id='today-min'></div> </div> </div> <div id='tomorrow'> <div id='tomorrow-day'></div> <canvas id='tomorrow-skycon' width ='50' height='50'></canvas> <div class='temperature-container'> <div id='tomorrow-max'></div> <div id='tomorrow-min'></div> </div> </div> <div id='next-day'> <div id='next-day-day'></div> <canvas id='next-day-skycon' width ='50' height='50'></canvas> <div class='temperature-container'> <div id='next-day-max'></div> <div id='next-day-min'></div> </div> </div> </div>";
  }
//If the current weather should be shown, add its markup
  if ($conf['current'] == 1) {
    $current_content = '<div id="current-weather"><canvas id="skycon" width="100" height="100"></canvas><div id="temp"></div><div id="feelslike"></div><div id="currently"></div></div>';
  }
//If the 24 hour data should be shown, add its markup
  if ($conf['24_hour'] == 1) {
    $twenty_four_content = '<div id="summary"></div>';
  }
//The markup for the block should now only be the configuration options that were selected
  $block->content = array(
    '#prefix' => '<div id="forecastdata">',
    '#markup' => $current_content . $twenty_four_content . $three_day_content,
    '#suffix' => '</div>',
  );

  libraries_load('skycons');

  if (($library = libraries_load('skycons')) && !empty($library['loaded'])) {
    // if skycons library is loaded, also load in the weather javascript
    drupal_add_js(drupal_get_path('module', 'signage_uiowa_panes') . '/js/weather.js');
  }

  return $block;
}
