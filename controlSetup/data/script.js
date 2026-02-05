// app.js - mínimo JS
const colorPicker = document.getElementById('colorPicker');
const valR = document.getElementById('valR');
const valG = document.getElementById('valG');
const valB = document.getElementById('valB');
const status = document.getElementById('status');

// Configuración
const ENDPOINT = '/RGB'; 
const PWM_MAX = 1023;

// Debounce simple para no spamear el servidor
let sendTimeout = null;
function debounceSend(r,g,b,delay=150){
  if(sendTimeout) clearTimeout(sendTimeout);
  sendTimeout = setTimeout(()=> sendRGB(r,g,b), delay);
}

// Convierte hex (#RRGGBB) a 0-1023
function hexTo1023(hex){
  if(hex.startsWith('#')) hex = hex.slice(1);
  const r = parseInt(hex.slice(0,2),16);
  const g = parseInt(hex.slice(2,4),16);
  const b = parseInt(hex.slice(4,6),16);
  const scale = PWM_MAX / 255;
  return {
    r: Math.round(r * scale),
    g: Math.round(g * scale),
    b: Math.round(b * scale)
  };
}

//envio generico
function enviarValorP(url, lampara, estado){
	fetch(url, {
		method: 'POST',
		headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
		body: 'lampara=' + lampara + '&estado=' + estado
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

// Envío usando application/x-www-form-urlencoded con r,g,b
function sendRGB(r,g,b){
  // Validación básica
  if([r,g,b].some(v => isNaN(v) || v < 0 || v > PWM_MAX)){
    status.textContent = 'Valores fuera de rango';
    return;
  }

  const body = 'r=' + encodeURIComponent(r) + '&g=' + encodeURIComponent(g) + '&b=' + encodeURIComponent(b);

  fetch(ENDPOINT, {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  })
  .then(resp => resp.text().then(t => ({ ok: resp.ok, status: resp.status, text: t })))
  .then(result => {
    if(result.ok){
      status.textContent = `Enviado: r=${r} g=${g} b=${b} — respuesta: ${result.text}`;
    } else {
      status.textContent = `Error ${result.status}: ${result.text}`;
    }
  })
  .catch(err => {
    console.error('Error al enviar:', err);
    status.textContent = 'Error de red al enviar color';
  });
}

// Actualiza UI y programa envío
function handleColorChange(e){
  const hex = e.target.value;
  const { r, g, b } = hexTo1023(hex);
  valR.textContent = r;
  valG.textContent = g;
  valB.textContent = b;
  debounceSend(r,g,b);
}

// Inicializa valores al cargar
(function init(){
  const initial = colorPicker.value || '#ffffff';
  const { r,g,b } = hexTo1023(initial);
  valR.textContent = r; valG.textContent = g; valB.textContent = b;
  // Enviar inicial opcional: comentar si no quieres envío automático al cargar
  // sendRGB(r,g,b);
})();

colorPicker.addEventListener('input', handleColorChange);

// botones
document.getElementById('btnLamPOn').addEventListener('click', ()=> {
  enviarValorP('/encendido', "lamP", 1);
});
document.getElementById('btnLamSOn').addEventListener('click', ()=> {
  enviarValorP('/encendido', "lamS", 1);
});
document.getElementById('btnRGBOn').addEventListener('click', ()=> {
  // ejemplo: encender RGB al máximo
  sendRGB(PWM_MAX, PWM_MAX, PWM_MAX);
});

// Ejemplo: botones de la pestaña 1 (envían comandos simples)
document.getElementById('btnLamPOff').addEventListener('click', ()=> {
  enviarValorP('/encendido', "lamP", 0);
});
document.getElementById('btnLamSOff').addEventListener('click', ()=> {
  enviarValorP('/encendido', "lamS", 0);
});
document.getElementById('btnRGBOff').addEventListener('click', ()=> {
  // ejemplo: encender RGB al máximo
  sendRGB(0, 0, 0);
});
