"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();

$(document).ready(function(){

    var score = 0;

    /*$(document).on("keypress", (evt)=>{
        cheeseJumping();
    });*/
	socket.on('jump', function(result){
		cheeseJumping();
	});
    var gameOver = setInterval(()=>{
        var cheeseTop = parseInt($('#chesseCharacter').css('top'));
        var mouseLeft = parseInt($('#mouseBlock').css('left'));
        if(mouseLeft <= 160 && mouseLeft > 16 && cheeseTop >= 320){
            //alert("hit " + mouseLeft  + " " + cheeseTop + " " + score)
        	var mouseLeft = parseInt($('#mouseBlock').css('left'));
            /*$("#mouseBlock").css("animation", "none");
            $("#mouseBlock").css("display", "none");
            clearInterval(countScore);
            clearInterval(gameOver);
            alert("Game Over! Final Score " + score);*/
			console.log("hit")
        }
    },5);

    var countScore = setInterval(()=>{
        score++;
        $('#score').text(score);
    }, 2000)
});

function cheeseJumping(){
    if(!$('#chesseCharacter').hasClass('jump')){
        $('#chesseCharacter').addClass('jump');
        setTimeout(() => {
            $('#chesseCharacter').toggleClass('jump');
        }, 500);
         
    }
}

function sendGameCommand(message) {
	socket.emit('game', message);
};