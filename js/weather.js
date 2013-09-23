/**
 * @file
 * Javascript for the signage panes module.
 */

 // Namespace jQuery to avoid conflicts.
(function($) {
  // Define the behavior.
  Drupal.weather = function() {

    // Call updateWeather every 5 minutes.
    setInterval(Drupal.weather.updateWeather, 300000);
    //setInterval(Drupal.weather.updateWeather, 10000);
  };

  // Attach weather behavior.
  Drupal.behaviors.weather = {
    attach: function(context, settings) {
      $('.pane-weather', context).once('weather', function() {
        Drupal.weather.updateWeather();
        Drupal.weather();
      });
    }
  };

  // Update date and time on the page
  Drupal.weather.updateWeather = function() {
    $.ajax({
      cache : false,
      type : 'GET',
      url : 'http://m.uiowa.edu/data/forecast/forecast.json',
      dataType : 'json',
      error : function() {
      },
      success : function parseResponse(json) {
        console.log(json);
        $('#temp').text(Math.round(json.currently.temperature)+'°');
        //$('#feelslike').text('Feels Like \n'+Math.round(json.currently.apparentTemperature)+'°');
        //$('#currently').text(json.currently.summary);
        $('#currently').text(json.currently.summary + ' - Feels Like \n' + Math.round(json.currently.apparentTemperature)+'°');
        $('#summary').html('<h3 class="24hours">Next 24 hours</h3><p>' + json.hourly.summary + '</p>');

        var icon = json.currently.icon;

        getSkycon(icon);

      }
    });

    function getSkycon(icon) {
      var skycons = new Skycons({"color": "white"});
      skycons.play();
      switch (icon) {
        case "clear-day":
          skycons.add("skycon", Skycons.CLEAR_DAY);
          break;
        case "clear-night":
          skycons.add("skycon", Skycons.CLEAR_NIGHT);
          break;
        case "rain":
          skycons.add("skycon", Skycons.RAIN);
          break;
        case "snow":
          skycons.add("skycon", Skycons.SNOW);
          break;
        case "sleet":
          skycons.add("skycon", Skycons.SLEET);
          break;
        case "wind":
          skycons.add("skycon", Skycons.WIND);
          break;
        case "fog":
          skycons.add("skycon", Skycons.FOG);
          break;
        case "cloudy":
          skycons.add("skycon", Skycons.CLOUDY);
          break;
        case "partly-cloudy-day":
          skycons.add("skycon", Skycons.PARTLY_CLOUDY_DAY);
          break;
        case "partly-cloudy-night":
          skycons.add("skycon", Skycons.PARTLY_CLOUDY_NIGHT);
          break;
        default:
          break;
      }
    }
  };

})(jQuery);
