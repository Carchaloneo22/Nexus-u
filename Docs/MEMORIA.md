# MEMORIA - Proyecto UnrealMCP (NexusU)

**Fecha:** 2026-05-26  
**Engine:** Unreal Engine 5.7  
**Repo:** `C:\Users\snayl\Documents\Unreal Projects\UnrealMCP`

---

## 1. ESTADO ACTUAL

### Proyecto re-organizado
- Mapa principal: `Content/NexusU/Maps/islas.umap`
- Isla 2: `Content/World/Isla2/` (terreno + edificios)
- Reorg completado: Assets sueltos movidos a carpetas específicas. Nuevas carpetas:
  - `Content/Interaction/Puzzles/DoorIF/`
  - `Content/Interaction/Puzzles/NetworkGame/`
  - `Content/Interaction/Terminals/ChatBot/`
  - `Content/Blueprints/Core/`
  - `Content/World/Props/`

### MCP configurado (Flopperam avanzado)
- Plugin `UnrealMCP` migrado de experimental → `unreal-engine-mcp-main` (avanzado, con Blueprint graph nodes, world building, etc.)
- Python server avanzado configurado en `opencode.jsonc`
- Backup del proyecto: `UnrealMCP_backup_20260630`

### Assets implementados y funcionando

| Asset | Ruta | Estado |
|---|---|---|
| **WBP_Puzzle_IF** | `Content/UI/Puzzles/` | Listo. Clase padre: `UserWidget`. Tutorial IF (condicional visual). |
| **BP_PuzzleTerminal_IF** | `Content/Interaction/Puzzles/DoorIF/` | Listo. Abre widget, escucha `OnPuzzleSolved`. |
| **BP_Puerta_IF** | `Content/Interaction/Puzzles/DoorIF/` | Listo. Recibe evento `AbrirPuerta`, rota 90°. |
| **BP_Puente_IF** | `Content/Interaction/Puzzles/DoorIF/` | Listo. Recibe evento `MostrarPuente`. |
| **BP_ChatTerminal_IS** | `Content/Interaction/Terminals/ChatBot/` | Existe. Abre `WBP_ChatBot_IS`. |
| **WBP_ChatBot_IS** | `Content/UI/ChatBot/` | Existe. C++ `Find Chat Bot Response` disponible en BP. Widgets: `TXT_Input`, `TXT_Output`, `BTN_Enviar`. **Falta conectar nodos en Event Graph.** |
| **BP_MiniJuego_Redes** | `Content/Interaction/Puzzles/NetworkGame/` | Creado. Lógica ahora en C++ (`AMiniJuegoRedes`). Falta BP hija + Widget. |
| **DT_FAQ_IS** | `Content/Data/ChatBot/` | DataTable vieja. Reemplazada por JSON + C++. |
| **FS_ChatBotFAQ** | `Content/Data/ChatBot/` | Struct viejo. Ahora solo referencia. |

### Nuevos C++ minijuegos (compilados)
| Clase | Archivos | Función |
|---|---|---|
| `AMiniJuegoRedes` | `MiniJuegoRedes.h/.cpp` | Secuencia correcta (modem→router→PC). Eventos: OnSequenceCorrect, OnSequenceWrong, OnItemAdded. |
| `APSeIntQuiz` | `PSeIntQuiz.h/.cpp` | Quiz de pseudocódigo. Array de `FPSeIntQuestion`. Eventos: OnQuestionAnswered, OnQuizFinished. |
| `AGridComandos` | `GridComandos.h/.cpp` | Grid 5x5. Movimientos: Up/Down/Left/Right. Eventos: OnMoved, OnGoalReached. |

### Plugins activos
- `UnrealMCP` (MCP avanzado Flopperam)
- `RemoteControl`, `MotionTrajectory`, etc. (ver `.uproject`)

---

## 2. CHATBOT UT ORIENTE (INGENIERÍA DE SOFTWARE)

### Datos
- **56 FAQs** en `Content/Data/ChatBot/FAQ_IS_Completo_UTOriente.json`
- Categorías: Saludos, Ayuda, General, Precio, Contacto, Requisitos, Plan (22 por semestre).

### Lógica implementada en C++

**Archivos:**
| Archivo | Ruta |
|---|---|
| `ChatBotSubsystem.h` | `Source/MCPGame/Public/` |
| `ChatBotSubsystem.cpp` | `Source/MCPGame/Private/` |
| `ChatBotFunctionLibrary.h` | `Source/MCPGame/Public/` |
| `ChatBotFunctionLibrary.cpp` | `Source/MCPGame/Private/` |

**Build.cs agregó módulos:** `Json`, `JsonUtilities`, `Projects`, `UMG`, `Slate`, `SlateCore`

**Funciones expuestas a Blueprint:**
- `Find Chat Bot Response(WorldContextObject, UserInput)` → `BlueprintCallable` en `ChatBotFunctionLibrary`.
- Auto-carga JSON en `UChatBotSubsystem::Initialize()` desde `Content/Data/ChatBot/FAQ_IS_Completo_UTOriente.json`.

### Conexión BP (WBP_ChatBot_IS)

```
On Clicked (BTN_Enviar)
  |
  ├── Get Text (TXT_Input) → InputText
  |
  ├── Find Chat Bot Response
  |     World Context Object: Self (o vacío)
  |     User Input: InputText
  |     Return Value → Respuesta
  |
  ├── Append (String)
  |     A: Get Text (TXT_Output)
  |     B: "\n\nTu: " + InputText + "\nBot: " + Respuesta
  |     → Set Text (TXT_Output)
  |
  └── Set Text (TXT_Input) = ""
```

**Problema resuelto:** `Find Chat Bot Response` no aparecía en BP porque `.uproject` faltaba array `Modules`. Solución: agregar `"Modules": [{ "Name": "MCPGame", "Type": "Runtime", "LoadingPhase": "Default" }]`, regenerar project files, reabrir Editor.

---

## 3. MINIJUEGO PUERTA IF (ISLA 1)

### Widget: WBP_Puzzle_IF
- Visualiza flujo: Inicio → Branch IF → True/False → Acción.
- `ComboBoxString` (`CMB_Condicion`): opciones `"La puerta está cerrada"` (default) / `"La puerta está abierta"`.
- `BTN_Verificar`: compara combo con `"La puerta está cerrada"`.
  - **True:** TXT_Resultado verde/amarillo, rama True resaltada, `BTN_AbrirPuerta` habilitado.
  - **False:** TXT_Resultado rojo, rama False resaltada, `BTN_AbrirPuerta` deshabilitado.
- `BTN_AbrirPuerta`: dispara `Event Dispatcher` `OnPuzzleSolved`, espera 2s, cierra widget.

### BP_PuzzleTerminal_IF
- Variables: `PuertaRef`, `PuenteRef`, `ActiveWidget`.
- `Interact` → Create Widget → Add to Viewport → Set Input Mode UI Only → Bind `OnPuzzleSolved`.
- `HandlePuzzleSolved` → Set Input Game Only → `AbrirPuerta` (PuertaRef) → `MostrarPuente` (PuenteRef) → limpia `ActiveWidget`.

### Assets 3D
- `BP_Puerta_IF`: Static Mesh, rota 90° en Y.
- `BP_Puente_IF`: Static Mesh, inicialmente hidden o scale 0, aparece al ganar.

---

## 4. MINIJUEGO REDES (ISLA 2)

- Asset creado: `BP_MiniJuego_Redes.uasset`
- **Lógica:** no implementada.
- Concepto: drag & drop de elementos (modem → router → PC) en orden correcto.

---

## 5. PRÓXIMOS PASOS PENDIENTES

| # | Tarea | Estado |
|---|---|---|
| 1 | **Conectar BP ChatBot** | Abrir `WBP_ChatBot_IS`. En Event Graph de `BTN_Enviar`: `Get Text(TXT_Input)` → `Find Chat Bot Response` → `Append` a `TXT_Output` → `Set Text(TXT_Input)=""`. Probar en PIE. |
| 2 | **Minijuego Redes BP + Widget** | Crear BP hija de `AMiniJuegoRedes`. Widget con botones (Modem, Router, PC) que llaman `AddItem`. Botón Verificar → `CheckSequence`. Mostrar resultado. |
| 3 | **Minijuego PSeInt BP + Widget** | Crear BP hija de `APSeIntQuiz`. Widget que muestra pregunta y opciones. Botones llaman `AnswerQuestion`. Mostrar puntaje final. |
| 4 | **Minijuego Grid BP + Widget** | Crear BP hija de `AGridComandos`. Widget con botones direccionales (Up/Down/Left/Right). Texto muestra posición actual. Al llegar a meta, mensaje de victoria. |
| 5 | **ChatBot: expandir FAQs** | 56 FAQs listas. Editar `FAQ_IS_Completo_UTOriente.json` si se necesitan más. |
| 6 | **Isla 3+** | No definido aún. |

---

## 6. PROBLEMAS Y SOLUCIONES REGISTRADOS

| Problema | Causa | Solución |
|---|---|---|
| `Find Chat Bot Response` no aparecía en BP | `.uproject` faltaba array `Modules` | Agregar `"Modules": [{...}]`, regenerar VS project files, reabrir Editor |
| "Missing target file" / "MCPGameEditor.target does not exist" | Botón "Compilar" en BP intenta compilar target inexistente | No usar botón Compilar de BP. Compilar desde command line: `Build.bat Development Win64 ...` |
| `Unable to build while Live Coding is active` | Editor abierto durante build C++ | Cerrar Editor antes de compilar, o usar `Ctrl+Alt+F11` |
| `C1083: No se puede abrir 'Engine/GameInstanceSubsystem.h'` | Include path incorrecto en UE5.7 | Cambiar a `"Subsystems/GameInstanceSubsystem.h"` |

---

## 7. COMANDOS ÚTILES

**Compilar C++ (cerrar Editor primero):**
```powershell
& "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" Development Win64 -Project="C:\Users\snayl\Documents\Unreal Projects\UnrealMCP\UnrealMCP.uproject" -TargetType=Editor -waitmutex
```

**Regenerar project files:**
```powershell
& "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="C:\Users\snayl\Documents\Unreal Projects\UnrealMCP\UnrealMCP.uproject" -game -engine -progress
```

**Launch project (batch existente):**
```batch
launch_project.bat
```

---

## 8. ARCHIVOS MD HISTÓRICOS (se mantienen como referencia)

| Archivo | Contenido |
|---|---|
| `Guia_EventGraph_Interactivo.md` | Nodos exactos del Event Graph de `WBP_Puzzle_IF`. Colores, flechas, Branch. |
| `Guia_Implementacion_Puerta_IF.md` | Guía paso a paso para crear minijuego IF (widget + puerta + puente + terminal). |
| `Guia_Integrar_ChatBot_UTOriente.md` | Guía original para integrar chatbot (enfoque BP + JSON). Reemplazada parcialmente por C++. |
| `Guia_ChatBot_Cpp_Connection.md` | Guía para conectar `UChatBotSubsystem` en BP. Quedó obsoleta con `Find Chat Bot Response`. |

---

## 9. ESTRUCTURA DE CÓDIGO C++

```
Source/MCPGame/
├── Public/
│   ├── MCPGame.h              # (stub)
│   ├── ChatBotSubsystem.h     # Subsystem + struct FAQEntry
│   ├── ChatBotFunctionLibrary.h # Función BlueprintCallable
│   ├── MiniJuegoRedes.h       # Actor: secuencia modem→router→PC
│   ├── PSeIntQuiz.h           # Actor: quiz pseudocódigo
│   └── GridComandos.h         # Actor: grid 5x5 con movimiento
├── Private/
│   ├── MCPGame.cpp            # (stub module)
│   ├── ChatBotSubsystem.cpp   # LoadFAQsFromJson + FindResponse
│   ├── ChatBotFunctionLibrary.cpp # Wrapper para acceso fácil en BP
│   ├── MiniJuegoRedes.cpp     # Lógica secuencia + eventos
│   ├── PSeIntQuiz.cpp         # Lógica quiz + eventos
│   └── GridComandos.cpp       # Lógica grid + eventos
└── MCPGame.Build.cs           # Dependencias: Core, Engine, Json, JsonUtilities, UMG, Slate, SlateCore
```

---

---

## 10. REORGANIZACIÓN DE ARCHIVOS Y MCP (Sesión 2026-06-30)

### Limpieza de repos MCP
- Repo viejo `unreal-mcp/` movido a `C:\Users\snayl\Documents\_BACKUPS\unreal-mcp_viejo\`.
- Repo activo: únicamente `unreal-engine-mcp-main\`.
- `Plugins\UnrealMCP` es junction → `unreal-engine-mcp-main\UnrealMCP`.

### Documentación centralizada
- Todas las guías `.md` movidas de raíz del proyecto a `Docs\`:
  - `MEMORIA.md`, `GUÍA_RÁPIDA.md`
  - `Guia_ChatBot_Cpp_Connection.md`
  - `Guia_EventGraph_Interactivo.md`
  - `Guia_Implementacion_Puerta_IF.md`
  - `Guia_Integrar_ChatBot_UTOriente.md`

### Nuevas carpetas Content creadas para sistema de puzzles genéricos
- `Content/Data/Puzzles/` → Data Assets `DA_PuzzleConfig` y sus instancias.
- `Content/UI/Puzzles/NodeEditor/` → Widgets `WBP_NodeEditor`, `WBP_PuzzleNode`, `WBP_Wire`.
- `Content/Interaction/Puzzles/NodePuzzles/` → Terminales de puzzles tipo nodo.
- `Content/Blueprints/Puzzles/` → Blueprints auxiliares de puzzles.

### Checklist MCP — Estado: OK
| Verificación | Resultado |
|---|---|
| Junction `Plugins\UnrealMCP` | OK → `unreal-engine-mcp-main\UnrealMCP` |
| `opencode.jsonc` | OK → apunta a Python avanzado |
| Editor corriendo | No (libre para compilar) |
| Python server test | OK (levanta sin errores) |
| Compilación C++ | OK (up to date, 2.3s) |

---

*Última actualización: sesión 2026-06-30. MCP consolidado, archivos reorganizados, proyecto listo para Sesión 1 de puzzles de nodos genéricos.*
