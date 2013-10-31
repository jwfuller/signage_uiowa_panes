/**
 * @file
 * Javascript for the signage panes module.
 */

// var ImageReloaded = 0;
// var BaseURL = "http://webcam.iowa.uiowa.edu/live/readImage.asp";
// var theTimer = setTimeout('reloadImage()', 1);
// function ImageLoaded() {
//   ImageReloaded = 1;
// }
// function reloadImage() {
//   theTimer = setTimeout('reloadImage()', 1000);
//   if (ImageReloaded = 1) {
//     theDate = new Date();
//     var url = BaseURL;
//     url += '?dummy=' + theDate.getTime().toString(10);
//     document.webCamImage.src = url;
//     ImageReloaded = 0;
//   }
// }

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
