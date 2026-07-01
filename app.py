from flask import Flask, render_template, request, redirect
from supabase import create_client, Client

app = Flask(__name__)

SUPABASE_URL = "https://occsdhjnvubnlauaooel.supabase.co"
SUPABASE_KEY = "sb_publishable_ESU7g0HbagxSNMGm3MMQTg_FLbwNwDq"

supabase: Client = create_client(SUPABASE_URL, SUPABASE_KEY)

estado_casa = {
    "sala": 0,
    "patio": 0,
    "clima": 0
}


@app.route("/")
def index():
    # Leer los últimos eventos de Supabase para mostrarlos en el monitor
    respuesta = supabase.table("registro_domotica").select("*").order("id", desc=True).limit(20).execute()
    eventos = respuesta.data
    return render_template("index.html", eventos=eventos)


@app.route("/activar/<dispositivo>", methods=["POST"])
def activar(dispositivo):
    if dispositivo in estado_casa:
        # Alternar el estado (si es 0 pasa a 1, si es 1 pasa a 0)
        estado_casa[dispositivo] = 1 if estado_casa[dispositivo] == 0 else 0
        
        accion_texto = "Encendido" if estado_casa[dispositivo] == 1 else "Apagado"
        
        # Guardar el evento en Supabase
        supabase.table("registro_domotica").insert({
            "dispositivo": dispositivo,
            "accion": accion_texto
        }).execute()
        
    return redirect("/")

@app.route("/api/estado", methods=["GET"])
def api_estado():
    # Devuelve un string simple como "1,0,1" para que el ESP32 lo procese fácil
    return f"{estado_casa['sala']},{estado_casa['patio']},{estado_casa['clima']}"


@app.route("/api/alerta", methods=["GET"])
def api_alerta():
    # Guardar el evento de alerta en Supabase
    supabase.table("registro_domotica").insert({
        "dispositivo": "Sensor Seguridad (ESP32-2)",
        "accion": "¡Alerta! Botón de pánico presionado"
    }).execute()
    
    return "Alerta recibida", 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
