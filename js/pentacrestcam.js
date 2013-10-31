/**
 * @file
 * Javascript for the signage panes module.
 */

 // Namespace jQuery to avoid conflicts.
(function($) {
  // Attach Pentacrest Cam behavior.
  Drupal.behaviors.pentacrestcam = {
    attach: function(context, settings) {
      $('.pane-pentacrestcam', context).once('pentacrestcam', function() {
        Drupal.pentacrestcam();
      });
    }
  };
  // Define the behavior.
  Drupal.pentacrestcam = function() {

    setInterval(function () {
      d = new Date();
      $('.webCamImage').attr("src", "http://webcam.iowa.uiowa.edu/live/readImage.asp?"+d.getTime());
    }, 1000);
  };

})(jQuery);
