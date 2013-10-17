/**
 * @file
 * Javascript for the Bongo panes module.
 */

 // Namespace jQuery to avoid conflicts.
(function($) {
  // Define the behavior.
  Drupal.itcAvailabilityGrid = function() {
    // Add an extra function to the Drupal ajax object which allows us to trigger
    // an ajax response without an element that triggers it.
    Drupal.ajax.prototype.specifiedResponse = function() {
      var ajax = this;

      // Do not perform another ajax command if one is already in progress.
      if (ajax.ajaxing) {
        return false;
      }

      try {
        $.ajax(ajax.options);
      }
      catch (err) {
        alert('An error occurred while attempting to process ' + ajax.options.url);
        return false;
      }

      return false;
    };

    // Call updateAvailability every two minutes.
    setInterval(Drupal.itcAvailabilityGrid.updateAvailability, 120000);
  };

  // Attach itcAvailabilityGrid behavior.
  Drupal.behaviors.itcAvailabilityGrid = {
    attach: function(context, settings) {
      $('.itc-availability-grid', context).once('itcAvailabilityGrid', function() {
        Drupal.itcAvailabilityGrid();
      });
    }
  };

  // Update all Availability on the page
  Drupal.itcAvailabilityGrid.updateAvailability = function() {
    var Availability = Drupal.settings.itcAvailabilityGrid;
    for (i = 0; i < Availability.length; i++) {
      itc = Availability[i];
      id = itc.id;

      // Define a custom ajax action not associated with an element.
      var custom_settings = {};
      custom_settings.url = Drupal.settings.basePath + 'itcavailability_panes_ajax_link_callback/ajax/' + id + '/';
      custom_settings.event = 'custom';
      custom_settings.keypress = false;
      custom_settings.prevent = false;
      custom_settings.progress = {'type': 'none'};
      Drupal.ajax['itc_availability_update'] = new Drupal.ajax(null, $("#ajax_link_itc-id-" + id), custom_settings);
      // Trigger the response.
      Drupal.ajax['itc_availability_update'].specifiedResponse();
    }
  };

})(jQuery);
