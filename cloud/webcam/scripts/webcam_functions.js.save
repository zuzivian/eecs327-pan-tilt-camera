var host = "http://35.208.31.153/img/";
var newImage = new Image();


function updateImage()
{
	document.getElementById("image").src = newImage.src;
	newImage = new Image();
	newImage.src = host + "camera.jpeg?t=" + Date.now();
	setTimeout(updateImage, 200);
}

document.addEventListener("DOMContentLoaded", updateImage);
console.log('starting webcam script');
