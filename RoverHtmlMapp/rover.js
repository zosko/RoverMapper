lines = [];

canvas = document.getElementById('roverJourney');
context = canvas.getContext("2d");

centerX = canvas.width / 2;
centerY = canvas.height / 2;

previousX = centerX;
previousY = centerY;

previousAngle = 0;

function handlePathCsvFile(files) {
  if (window.FileReader) {      
      getAsText(files[0]);
  } else {
      alert('FileReader are not supported in this browser.');
  }
}

function getAsText(fileToRead) {
  var reader = new FileReader();

  reader.readAsText(fileToRead);

  reader.onload = loadHandler;
  reader.onerror = errorHandler;
}

function loadHandler(event) {
  var csv = event.target.result;
  processDataAndDrawPath(csv);

	drawRoverPoints();
}

function processDataAndDrawPath(csv) {
    var allTextLines = csv.split(/\r\n|\n/);
    
    for (var i=0; i<allTextLines.length; i++) {
        var data = allTextLines[i];

	    lines.push(data);
    }
}

function errorHandler(evt) {
  if(evt.target.error.name == "NotReadableError") {
      alert("Canno't read file !");
  }
}

function drawRoverPoints() {	
	var step = 5;

	for (var i=0; i<lines.length; i++) {
		context.moveTo(previousX, previousY);

		if (lines[i] == 'c') {
		    context.beginPath();
		    context.arc(previousX, previousY, 10, 0, 2 * Math.PI, false);
		    context.fillStyle = 'red';
		    context.fill();		  
		    context.stroke();
			
			continue;
		};			

		// if (lines[i] != previousAngle) {
		//     context.beginPath();
		//    	context.arc(previousX, previousY, 5, 0, 2 * Math.PI, false);
		//    	context.fillStyle = 'green';
		//     context.fill();		  
		//     context.stroke();

		// 	context.fillText(lines[i],previousX + 10,previousY + 10);	
		// }

		previousAngle = lines[i];

		previousX = previousX + (step * Math.cos(degreesToRadians(lines[i] * -1)));
		previousY = previousY + (step * Math.sin(degreesToRadians(lines[i] * -1)));


		context.lineTo(previousX, previousY);
		context.stroke();
	}
}

function degreesToRadians(degrees) {
    return (degrees * Math.PI)/180;
}
