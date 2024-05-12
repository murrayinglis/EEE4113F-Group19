// Create Temperature Chart
var chartT = new Highcharts.Chart({
  chart:{
    renderTo:'chart-temperature'
  },
  series: [
    {
      name: 'Temperature',
      type: 'line',
      color: '#101D42',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#101D42',
      }
    }
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: {
      text: 'Temperature Celsius Degrees'
    }
  },
  credits: {
    enabled: false
  }
});

// Define temperature data points
var temperatureData = [
  { time: '10:01', temperature: 32 },
  { time: '10:05', temperature: 35 },
  { time: '10:10', temperature: 38 }
];

// Function to get current readings on the webpage when it loads for the first time
function getReadings() {
  temperatureData.forEach(function(data) {
    plotTemperature(data.temperature, data.time);
  });
}

// Function to plot temperature on the temperature chart
function plotTemperature(temperature, time) {
  var x = new Date('2024-05-01T' + time + ':00').getTime();
  if (chartT.series[0].data.length > 40) {
    chartT.series[0].addPoint([x, temperature], true, true, true);
  } else {
    chartT.series[0].addPoint([x, temperature], true, false, true);
  }
}

// Get current sensor readings when the page loads
window.addEventListener('load', getReadings);
