//variables
let intervals = {}; // objeto para guardar intervalos por tipo
let startTimes = {};
let activeButtons = {};

const frenteBtn = document.getElementById('frente');
const atrasBtn = document.getElementById('atras');
const derechaBtn = document.getElementById('derecha');
const izquierdaBtn = document.getElementById('izquierda');

function enviarValorP(url, valor){
	fetch(url, {
		method: 'POST',
		headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
		body: 'value=' + valor
	})
	.then(response => response.text())
	.then(data => {
		// eslint-disable-next-line no-console
		console.log('Respuesta servidor: ', data);
	})
	.catch(err =>{ 
		// eslint-disable-next-line no-console
		console.error('Error al enviar: ', err);
	});
}

function obtenerValorG(url, destinoId){
	fetch(url, {method: 'GET'})
	.then(response => response.json())
	.then(data => {
		document.getElementById(destinoId).textContent = data.bateria+'%';
	})
	.catch(err => {
		document.getElementById(destinoId).textContent = 'Error al obtener: ' + err;
	});
}

// función genérica para iniciar movimiento
function startMove(url, tipo){
  startTimes[tipo] = Date.now();
  activeButtons[tipo] = true;   // <-- nuevo

  if (intervals[tipo]) return;

  intervals[tipo] = setInterval(() => {
    // si frente y derecha están activos al mismo tiempo
    if (activeButtons['F'] && activeButtons['R']) {
      enviarValorP(url, 'I'); // enviar I
    } else if (activeButtons['F'] && activeButtons['L']) {
      enviarValorP(url, 'G'); // enviar I
    } else if (activeButtons['B'] && activeButtons['R']) {
      enviarValorP(url, 'J'); // enviar I
    } else if (activeButtons['B'] && activeButtons['L']) {
      enviarValorP(url, 'H'); // enviar I
    } else {
      if (tipo === 'F' || tipo === 'B') {
        const elapsed = (Date.now() - startTimes[tipo]) / 1000;
        let n = Math.min(9, Math.floor(elapsed / 0.5) + 1);
        enviarValorP(url, tipo + n);
      } else {
        enviarValorP(url, tipo);
      }
    }
  }, 200);
}

function stopMove(url, tipo){
  activeButtons[tipo] = false;   // <-- nuevo

  if (intervals[tipo]) {
    clearInterval(intervals[tipo]);
    delete intervals[tipo];
  }

  if (tipo === 'F' || tipo === 'B') {
    enviarValorP(url, tipo + 'S0');
  }
}

//asociacionluzF
document.getElementById('luzOn').addEventListener('click', () => {
	enviarValorP('/luz', 1);
});

document.getElementById('luzOff').addEventListener('click', () => {
	enviarValorP('/luz', 0);
});

//asociacionBuzF
document.getElementById('buzOn').addEventListener('click', () => {
	enviarValorP('/buz', 1);
});

document.getElementById('buzOff').addEventListener('click', () => {
	enviarValorP('/buz', 0);
});

// asociar botones
frenteBtn.addEventListener('mousedown', () => startMove('/mov','F'));
frenteBtn.addEventListener('mouseup', () => stopMove('/mov','F'));
frenteBtn.addEventListener('touchstart', e => { e.preventDefault(); startMove('/mov','F'); });
frenteBtn.addEventListener('touchend', () => stopMove('/mov','F'));

atrasBtn.addEventListener('mousedown', () => startMove('/mov','B'));
atrasBtn.addEventListener('mouseup', () => stopMove('/mov','B'));
atrasBtn.addEventListener('touchstart', e => { e.preventDefault(); startMove('/mov','B'); });
atrasBtn.addEventListener('touchend', () => stopMove('/mov','B'));

derechaBtn.addEventListener('mousedown', () => startMove('/mov','R'));
derechaBtn.addEventListener('mouseup', () => stopMove('/mov','R'));
derechaBtn.addEventListener('touchstart', e => { e.preventDefault(); startMove('/mov','R'); });
derechaBtn.addEventListener('touchend', () => stopMove('/mov','R'));

izquierdaBtn.addEventListener('mousedown', () => startMove('/mov','L'));
izquierdaBtn.addEventListener('mouseup', () => stopMove('/mov','L'));
izquierdaBtn.addEventListener('touchstart', e => { e.preventDefault(); startMove('/mov','L'); });
izquierdaBtn.addEventListener('touchend', () => stopMove('/mov','L'));
