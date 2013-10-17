<?php
/**
 * @file
 * Ctools content type plugin file for ITC availability grid.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('ITC Availability Grid (Multiple)'),
  'description' => t('Shows a grid of ITC availability.'),
  'category' => t('UIowa'),
  'edit form' => 'itcavailabilitygrid_edit_form',
  'render callback' => 'itcavailabilitygrid_render',
  'admin info' => 'itcavailabilitygrid_admin_info',
  'defaults' => array(
    'location' => array(194 => 194, 393 => 393),
    'show_refresh' => 1,
  ),
);

/**
 * The 'admin info' callback for panel pane.
 */
function itcavailabilitygrid_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Showing ITC availability information, in a grid.');
    return $block;
  }
}

/**
 * The 'Edit form' callback for the content type.
 */
function itcavailabilitygrid_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $options = itcavailabilitygrid_build_itc_options();
  $form['location'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#options' => $options,
    '#multiple' => TRUE,
    '#size'=> 30,
    '#default_value' => $conf['location'],
    '#description' => t("The ITC locations you wish to display. Please select at least two options in order for this to render correctly."),
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
function itcavailabilitygrid_edit_form_submit($form, &$form_state) {
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
function itcavailabilitygrid_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();
  $ids = $conf['location'];
  $ids_string = implode('-', $ids);

  $block->title = 'ITC Computer Availability';
  $block->content = array(
    array(
      itc_build_availability_table($conf['location']),
    ),
    array(
      itcavailability_panes_ajax_render($ids_string, $conf['show_refresh']),
    ),
  );

  drupal_add_js(
    array(
      'itcAvailabilityGrid' => array(
        array(
          'id' => $ids_string,
        ),
      ),
    ), 'setting');
  drupal_add_js(drupal_get_path('module', 'signage_uiowa_panes') . '/js/itcavailabilitygrid.js');
  return $block;
}

/**
 * Build an array of options for the configuration form.
 */
function itcavailabilitygrid_build_itc_options() {
  $data = _signage_uiowa_panes_decode('http://cs-services.its.uiowa.edu/webservices/inventory/locationshort?format=json&type=ITC');
  foreach ($data['Locations'] as $location) {
    $id  = $location['ID'];
    $title = $location['DisplayName'];
    $options[$id] = t('@title', array('@title' => $title));
  }
  return $options;
}
