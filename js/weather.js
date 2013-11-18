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
        //$('#feelslike').text('Feels Like \n'+Math.round(json.currently.apparentTemperature)+'°');
        //$('#currently').text(json.currently.summary);

        //Check if the markup for each of the options exists.
        //If it does, add the relevant information to the markup
        if ($('#current-weather').length > 0) {
          $('#temp').text(Math.round(json.currently.temperature)+'°');
          $('#currently').text(json.currently.summary + ' - Feels Like \n' + Math.round(json.currently.apparentTemperature)+'°');
          var icon = json.currently.icon;
          getSkycon(icon,"skycon")
        };
        if ($('#summary').length > 0) {
          $('#summary').html('<h3 class="24hours">Next 24 hours</h3><p>' + json.hourly.summary + '</p>');
        };

        if ($('#three-day-forecast').length > 0) {
          var forecastData = getExtendedForecast(json.daily.data);
          var todayIcon = forecastData[0].icon;
          var tomorrowIcon = forecastData[1].icon;
          var nextDayIcon = forecastData[2].icon;

          $('#today-day').text('Today');
          $('#today-max').text(forecastData[0].max);
          $('#today-min').text(forecastData[0].min);
          getSkycon(todayIcon,"today-skycon");

          $('#tomorrow-day').text(forecastData[1].day);
          $('#tomorrow-max').text(forecastData[1].max);
          $('#tomorrow-min').text(forecastData[1].min);
          getSkycon(tomorrowIcon,"tomorrow-skycon");

          $('#next-day-day').text(forecastData[2].day);
          $('#next-day-max').text(forecastData[2].max);
          $('#next-day-min').text(forecastData[2].min);
          getSkycon(nextDayIcon,"next-day-skycon");

        };


      }
    });

    function getSkycon(icon,element) {
      var skycons = new Skycons({"color": "white"});
      skycons.play();
      switch (icon) {
        case "clear-day":
          skycons.add(element, Skycons.CLEAR_DAY);
          break;
        case "clear-night":
          skycons.add(element, Skycons.CLEAR_NIGHT);
          break;
        case "rain":
          skycons.add(element, Skycons.RAIN);
          break;
        case "snow":
          skycons.add(element, Skycons.SNOW);
          break;
        case "sleet":
          skycons.add(element, Skycons.SLEET);
          break;
        case "wind":
          skycons.add(element, Skycons.WIND);
          break;
        case "fog":
          skycons.add(element, Skycons.FOG);
          break;
        case "cloudy":
          skycons.add(element, Skycons.CLOUDY);
          break;
        case "partly-cloudy-day":
          skycons.add(element, Skycons.PARTLY_CLOUDY_DAY);
          break;
        case "partly-cloudy-night":
          skycons.add(element, Skycons.PARTLY_CLOUDY_NIGHT);
          break;
        default:
          break;
      };
    };
//Helper function to parse JSON data for the 3 day forecast
    function getExtendedForecast(data) {
      var forecastData = new Array();
      //Go through the "daily" data from the JSON feed and pull out
      //relevant information for the first 3 days
      for(var i =0; i< 3; i++) {
        //Convert UNIX time to milliseconds
        var timeStamp = data[i].time * 1000;
        var day = new Date(timeStamp);
        forecastData[i] = {
          //Pulls out the first 3 letters of the day of the week
          'day': day.toUTCString().substring(0,3),
          'icon': data[i].icon,
          'min' : (Math.round(data[i].temperatureMin) + '°'),
          'max' : (Math.round(data[i].temperatureMax) + '°'),
        };
      };
      return forecastData;
    };
  };

})(jQuery);
