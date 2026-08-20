# GUÍA RÁPIDA - Próximos pasos en Unreal Editor

## 1. Abrir Unreal Editor

Ejecutar `launch_project.bat` o abrir `UnrealMCP.uproject` desde Epic Games Launcher.

## 2. Verificar Plugin MCP

Ir a `Edit → Plugins → Installed`. Buscar `UnrealMCP`. Debe estar **Enabled**.

Si aparece advertencia de compilar, ignorar (ya compiló desde PowerShell).

## 3. Conectar ChatBot (WBP_ChatBot_IS)

Abrir `Content/UI/ChatBot/WBP_ChatBot_IS`.

En **Event Graph**:

```
OnClicked (BTN_Enviar)
  |
  ├── GetText (TXT_Input) → InputText
  |
  ├── Find Chat Bot Response
  |     WorldContextObject: Self
  |     UserInput: InputText
  |     ReturnValue → Respuesta
  |
  ├── Append (String)
  |     A: GetText (TXT_Output)
  |     B: "\n\nTu: " + InputText + "\nBot: " + Respuesta
  |     → SetText (TXT_Output)
  |
  └── SetText (TXT_Input) = ""
```

Guardar. Probar en **PIE** (Play In Editor).

## 4. Crear BP hija de MiniJuegoRedes

1. En Content Browser, click derecho → `Blueprint Class`.
2. Elegir `MiniJuegoRedes` como padre.
3. Guardar en `Content/Interaction/Puzzles/NetworkGame/BP_MiniJuegoRedes_Logica`.
4. Abrir BP.
5. En **Event Graph**, usar `AddItem("Modem")`, `AddItem("Router")`, `AddItem("PC")` desde botones de un Widget.
6. Botón **Verificar** → `CheckSequence` → Branch por ReturnValue.
7. Bind `OnSequenceCorrect` → mostrar mensaje de victoria.
8. Bind `OnSequenceWrong` → mostrar mensaje de error.

## 5. Crear Widget para MiniJuegoRedes

1. Click derecho → `User Widget` → `WBP_MiniJuegoRedes`.
2. Guardar en `Content/UI/Puzzles/`.
3. Agregar botones: `BTN_Modem`, `BTN_Router`, `BTN_PC`, `BTN_Verificar`, `BTN_Reset`.
4. Bind OnClicked → cast a `BP_MiniJuegoRedes_Logica` → `AddItem` / `CheckSequence` / `ResetGame`.
5. Texto dinámico: `GetCurrentSequenceText`.

## 6. Crear BP hija de PSeIntQuiz

1. `Blueprint Class` → padre `PSeIntQuiz`.
2. Guardar en `Content/Interaction/Puzzles/PSeInt/BP_PSeIntQuiz_Logica`.
3. Editar `Questions` array en Defaults:
   - Pregunta 1: `QuestionText="¿Qué estructura repite hasta cumplir condición?"`, Options=`["Si","Mientras","Para","Segun"]`, CorrectIndex=`1`
   - Agregar más preguntas.
4. Widget `WBP_PSeIntQuiz` con texto pregunta, botones opciones, texto puntaje.

## 7. Crear BP hija de GridComandos

1. `Blueprint Class` → padre `GridComandos`.
2. Guardar en `Content/Interaction/Puzzles/Grid/BP_GridComandos_Logica`.
3. Widget `WBP_GridComandos` con botones Up/Down/Left/Right y texto posición.
4. Bind botones → `MoveUp` / `MoveDown` / `MoveLeft` / `MoveRight`.
5. Bind `OnGoalReached` → mensaje victoria.

## 8. Probar MCP desde OpenCode

Con Editor abierto, el plugin levanta server TCP en puerto 55557. OpenCode ahora tiene configurado el MCP server `unrealMCP`. Podés pedirle a OpenCode cosas como:

- "Spawn a cube at location 0,0,0"
- "Create a Blueprint named BP_Test in Content/Blueprints"
- "Add a Branch node in WBP_Puzzle_IF"

Si OpenCode no detecta el server, reiniciar OpenCode (cerrar y volver a abrir terminal).

## 9. Comandos útiles (si necesitás recompilar C++ de nuevo)

```powershell
# Cerrar Editor primero
& "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" Development Win64 -Project="C:\Users\snayl\Documents\Unreal Projects\UnrealMCP\UnrealMCP.uproject" -TargetType=Editor -waitmutex
```

---

**Nota:** No usar botón "Compile" de Blueprints para C++. Ese botón es para BP puros. C++ compila desde command line.
