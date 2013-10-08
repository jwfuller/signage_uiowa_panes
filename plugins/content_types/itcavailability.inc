<?php
/**
 * @file
 * Ctools content type plugin file for bongo module.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('ITC Availability'),
  'description' => t('Shows an ITC with computer availability.'),
  'category' => t('UIowa'),
  'edit form' => 'itcavailability_edit_form',
  'render callback' => 'itcavailability_render',
  'admin info' => 'itcavailability_admin_info',
  'defaults' => array(
    'location' => 194,
    'show_availability_text' => 1,
    'show_refresh' => 1,
  ),
);

/**
 * The 'admin info' callback for panel pane.
 */
function itcavailability_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Showing ITC availability information.');
    return $block;
  }
}

/**
 * The 'Edit form' callback for the content type.
 */
function itcavailability_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $options = itcavailability_build_itc_options();
  $form['location'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#options' => $options,
    '#default_value' => $conf['location'],
    '#description' => t("The ITC location."),
  );
  $form['show_availability_text'] = array(
    '#title' => t('Show Availability Text'),
    '#description' => t('Show the "Available Computers" text under the number of computers.'),
    '#type' => 'checkbox',
    '#default_value' => $conf['show_availability_text'],
  );
  $form['show_refresh'] = array(
    '#title' => t('Show Refresh link'),
    '#description' => t('Show the "Refresh availability" link at the bottom.'),
    '#type' => 'checkbox',
    '#default_value' => $conf['show_refresh'],
  );

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function itcavailability_edit_form_submit($form, &$form_state) {
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
function itcavailability_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();
  // Get data from the ITC feed.
  $data = itc_data($conf['location']);
  $title = $data['title'];
  $id = $data['id'];
  $available = $data['total_available'];
  $total = $data['total_computers'];

  $block->title = $title;
  $block->content = array(
    array(
      itc_availability($id, $available, $total),
    ),
    array(
      _itc_availability_text($conf['show_availability_text']),
    ),
    array(
      itcavailability_panes_ajax_render($id, $conf['show_refresh']),
    ),
  );

  drupal_add_js(
    array(
      'itcAvailability' => array(
        array(
          'id' => $id,
        ),
      ),
    ), 'setting');
  drupal_add_js(drupal_get_path('module', 'signage_uiowa_panes') . '/js/itcavailability.js');
  return $block;
}

/**
 * Build an array of options for the configuration form.
 */
function itcavailability_build_itc_options() {
  $data = _signage_uiowa_panes_decode('http://cs-services.its.uiowa.edu/webservices/inventory/locationshort?format=json&type=ITC');
  foreach ($data['Locations'] as $location) {
    $id  = $location['ID'];
    $title = $location['DisplayName'];
    $options[$id] = t('@title', array('@title' => $title));
  }
  return $options;
}

/**
 * Determines if ITC availability text should be shown.
 *
 *  @param int $show_availability_text
 *   Determines if the availability text should be shown.
 *   If 1 then it is shown, if 0 then it is not shown.
 *
 * @return array
 *   The markup with the availability text.
 */
function _itc_availability_text($show_availability_text) {
  if ($show_availability_text == 1) {
    return array('#markup' => '<span class="availability-text">Available Computers</span>');
  }
  else {
    return;
  }
}
