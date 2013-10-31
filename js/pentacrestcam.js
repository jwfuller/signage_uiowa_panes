var ImageReloaded = 0;
var BaseURL = "http://webcam.iowa.uiowa.edu/";
// This is the path to the image generating asp file.
var File = "live/readImage.asp";
// Force an immediate image load
var theTimer = setTimeout('reloadImage()', 1);
// This function will stop unneeded reloads if client has slow bandwidth
function ImageLoaded() {
  ImageReloaded = 1;
}
function reloadImage() {
  // Reload the image every one second (1000 ms)
  theTimer = setTimeout('reloadImage()', 1000);
  // Here we load the image
  if (ImageReloaded=1) {
    theDate = new Date();
    var url = BaseURL;
    url += File;
    url += '?dummy=' + theDate.getTime().toString(10);
    // The dummy above enforces a bypass of the browser image cache
    document.webCamImage.src = url;
    ImageReloaded = 0;
  }
}
