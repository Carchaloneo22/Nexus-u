# Guia: Conectar ChatBot C++ en Blueprint

## RESUMEN

Codigo C++ compilado y listo. `UChatBotSubsystem` carga `FAQ_IS_Completo_UTOriente.json` automaticamente al iniciar el juego. Solo falta conectar 3 nodos en Blueprint para usarlo.

---

## PASO 1: OBTENER EL SUBSYSTEM EN BLUEPRINT

```
Click derecho en Event Graph → buscar:
"Get Game Instance" → Return Value (Game Instance)
    |
    └──→ "Get Subsystem" (clase: UChatBotSubsystem)
          Return Value (Chat Bot Subsystem Reference)
```

**Nota:** Si "Get Subsystem" no aparece, usa "Get Game Instance Subsystem" y selecciona `ChatBotSubsystem` en el dropdown.

---

## PASO 2: ENVIAR MENSAJE Y MOSTRAR RESPUESTA

En `WBP_ChatBot_IS` o `BP_ChatTerminal_IS`:

```
Event OnClicked (BTN_Enviar)              [o OnTextCommitted (TXT_Input)]
    |
    ├──→ Get Text (TXT_Input) → TextoUsuario
    |
    ├──→ Get Game Instance
    |     └──→ Get Subsystem (ChatBotSubsystem)
    |           └──→ Find Response
    |                 Input: TextoUsuario
    |                 Return Value: RespuestaBot (String)
    |
    ├──→ Append a TXT_Output:
    |     TXT_Output += "\nTu: " + TextoUsuario
    |     TXT_Output += "\nBot: " + RespuestaBot
    |
    ├──→ Set Text (TXT_Input) = "" (limpiar input)
    |
    └──→ Scroll To End (ScrollBox)
```

**Nodos exactos:**
- `Find Response` (Target: Chat Bot Subsystem Reference, User Input: String)
- `Set Text` (Target: TXT_Output)
- `Set Text` (Target: TXT_Input, Texto: vacio)

---

## PASO 3: AUTO-CARGA DEL JSON (YA ESTA HECHO)

El C++ ya carga el JSON automaticamente en `Initialize()`:
- Ruta: `Content/Data/ChatBot/FAQ_IS_Completo_UTOriente.json`
- Se ejecuta al inicio del GameInstance.

**Si quieres recargar manualmente:**
```
Get Game Instance → Get Subsystem (ChatBotSubsystem)
    └──→ Load FAQs From Json
          Json File Path: "[Ruta absoluta o relativa a .uproject]/Content/Data/ChatBot/FAQ_IS_Completo_UTOriente.json"
```

---

## CHECKLIST DE PRUEBA RAPIDA

1. Compilar C++ (ya hecho). Relanza Unreal Editor.
2. Abrir `WBP_ChatBot_IS` → Event Graph.
3. Conectar nodos del Paso 2.
4. Presionar **Play (PIE)**.
5. Escribir en chat: `precio`
6. Resultado esperado: `COP $2.200.000/semestre`
7. Escribir: `hola`
8. Resultado esperado: Saludo del bot UT Oriente.
9. Escribir: `primer semestre`
10. Resultado esperado: Lista de 6 materias del semestre 1.

---

## ERRORES COMUNES

| Error | Causa | Solucion |
|---|---|---|
| "Accessed None" en Subsystem | GameInstance no inicializado | Asegurar que llamas despues de BeginPlay |
| Respuesta: "Lo siento, el asistente aun no esta listo." | JSON no cargo | Verificar que `FAQ_IS_Completo_UTOriente.json` existe en `Content/Data/ChatBot/` |
| Subsystem no aparece en BP | C++ no compilo o Editor no reinicio | Cerrar Editor, compilar C++, abrir Editor |
| Respuesta vacia | Texto input no se paso bien | Verificar pin "User Input" en `Find Response` |

---

## ARCHIVOS C++ CREADOS

| Archivo | Ruta |
|---|---|
| Header | `Source/MCPGame/Public/ChatBotSubsystem.h` |
| Implementacion | `Source/MCPGame/Private/ChatBotSubsystem.cpp` |
| Build.cs modificado | `Source/MCPGame/MCPGame.Build.cs` |

---

## PROXIMO PASO

Minijuego Redes (Isla 2) o minijuego PSeInt. Cuando quieras.
