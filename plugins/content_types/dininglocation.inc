<?php
/**
 * @file
 * Ctools content type plugin file for individual ITC pane.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Dining Location Information'),
  'description' => t('Shows hours and open status for a dining location.'),
  'category' => t('UIowa'),
  'edit form' => 'dininglocation_edit_form',
  'render callback' => 'dininglocation_render',
  'admin info' => 'dininglocation_admin_info',
  'defaults' => array(
    'location' => 4,
    'show_summary_text' => 0,
    'show_building_text' => 0,
  ),
);

/**
 * The 'admin info' callback for panel pane.
 */
function dininglocation_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Showing Dining Location information.');
    return $block;
  }
}

/**
 * The 'Edit form' callback for the content type.
 */
function dininglocation_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $options = dininglocation_build_location_options();
  $form['location'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#options' => $options,
    '#default_value' => $conf['location'],
    '#description' => t("The Dining facility location."),
  );
  $form['show_summary_text'] = array(
    '#title' => t('Show Location Summary'),
    '#description' => t('Show the summary text for the location.'),
    '#type' => 'checkbox',
    '#default_value' => $conf['show_summary_text'],
  );
  $form['show_building_text'] = array(
    '#title' => t('Show Building Name'),
    '#description' => t('Show the name of the building that contains the location.'),
    '#type' => 'checkbox',
    '#default_value' => $conf['show_building_text'],
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function dininglocation_edit_form_submit($form, &$form_state) {
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
function dininglocation_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();
  // Get data from the Dining Locations feed.
  $data = location_data($conf['location']);
  $title = $data['title'];
  $id = $data['id'];
  $building = $data['building'];
  $hours = $data['hours_regular'];
  $summary = $data['summary'];

  $block->title = $title;
  $block->content = array(
    array(
      _dining_building_text($id, $building, $conf['show_building_text']),
    ),
    array(
      _dining_summary_text($id, $summary, $conf['show_summary_text']),
    ),
    array(
      dining_hours($id, $hours),
    ),
  );

  drupal_add_css(drupal_get_path('module', 'signage_uiowa_panes') . '/css/dininglocation.css');

  return $block;
}

/**
 * Build an array of options for the configuration form.
 */
function dininglocation_build_location_options() {
  $data = _signage_uiowa_panes_decode('http://dining.uiowa.edu/api/v1/json/locations');
  foreach ($data['locations'] as $location) {
    $id  = $location['location']['id'];
    $title = $location['location']['title'];
    $options[$id] = t('@title', array('@title' => $title));
  }
  return $options;
}

/*
 * Build an array of the output data.
 */

function location_data($id) {
  $feed = _signage_uiowa_panes_decode('http://dining.uiowa.edu/api/v1/json/locations');
  foreach ($feed['locations'] as $location) {
    if ($location['location']['id'] == $id) {
      $data['id'] = $location['location']['id'];
      $data['title'] = $location['location']['title'];
      $data['building'] = $location['location']['building'];
      $data['summary'] = $location['location']['summary'];
      // Replace + with <br />
      $data['hours_regular'] = str_replace("+", "<br />", $location['location']['hours_regular']);

      return $data;
    }
  }
}

function dining_hours($id, $hours) {
  return array('#markup' => '<div class="hours">' . $hours . '</div>',);
}

/**
 * Determines if text should be shown.
 *
 *  @param int $show_summary_text
 *  @param int $show_building_text
 *   If 1 then it is shown, if 0 then it is not shown.
 *
 * @return array
 *   The markup with the summary text.
 */
function _dining_summary_text($id, $summary, $show_summary_text) {
  if ($show_summary_text == 1) {
    return array('#markup' => '<div class="summary">' . $summary . '</div>');
  }
  else {
    return;
  }
}

function _dining_building_text($id, $building, $show_building_text) {
  if ($show_building_text == 1) {
    return array('#markup' => '<div class="building">' . $building . '</div>');
  }
  else {
    return;
  }
}
