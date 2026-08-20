# NexusU — Campus Virtual con Minijuegos Educativos (UE5)

Proyecto de grado en **Unreal Engine 5.8**. Campus universitario virtual con IA conversacional y **minijuegos educativos de Ingeniería de Software** para reforzar conceptos de la carrera (conceptos ↔ definiciones, metodologías ágiles, control de versiones, redes).

---

## Requisitos previos

| Herramienta | Versión mínima | Descarga |
|---|---|---|
| Unreal Engine | **5.8** | Epic Games Launcher |
| Git | cualquiera | https://git-scm.com |
| Git LFS | cualquiera | https://git-lfs.com |
| Visual Studio 2022 | Community | workload `Game Development with C++` (solo para compilar el plugin MCP) |

---

## Clonar el repositorio

```bash
git lfs install
git clone https://github.com/Huesly/UE5.git
cd UE5
git lfs pull
```

## Abrir el proyecto

1. Abrir `UnrealMCP.uproject` con UE 5.8.
2. Aceptar el popup de recompilación del plugin **UnrealMCP** (requiere VS2022).
3. Nivel principal: `Content/Maps/Firts`.

> **Nota:** el plugin `Plugins/UnrealMCP` y `ModelContextProtocol` están **deshabilitados del repo** (locales). Se instalan por separado — ver `Docs/`.

---

## Minijuegos educativos

| Minijuego | Qué enseña | Assets |
|---|---|---|
| **Memoria de conceptos** | Emparejar término ↔ definición (Testing, Refactoring, Scrum, REST, Git) | `WBP_NexusMemoryGame` |
| **Cableado de red** | Conexión de redes / rutas de datos | `BP_MG_NetworkCabling` + `WBP_MG_NetworkCabling` |
| **Puzzle de puerta** | Lógica de puzzles interactivos | `BP_PuzzleTerminal_IF` + `WBP_Puzzle_IF` |
| **ChatBot IA** | IA conversacional (FAQ de la universidad) | `BP_ChatTerminal_IS` + `WBP_ChatBot_Final` + `DT_FAQ_IS` |

### Minijuego de memoria (conceptos ↔ definiciones)

- Emparejá un término (Testing, Refactoring, Scrum, REST, Git) con su definición.
- **5 pares** = ganar · **3 vidas** o **60 segundos** = perder.
- Mecánica: `SrcArray[SourceIndex] == DstArray[Index]` con mapa fijo `DstArray = [4,2,0,3,1]`.

**Cómo cambiar las preguntas** (sin tocar lógica):
1. Editá el texto de `BTN_Source_0..4` (términos) y `BTN_Dest_0..4` (definiciones).
2. Actualizá el orden del array `DstArray` para re-mapear pares.

---

## Estructura del proyecto

```
Content/
├── Blueprints/          # BP generales + WBP_NexusMemoryGame
├── Core/
│   ├── GameMode/        # BP_ThirdPersonGameMode
│   ├── Input/           # IA_* (Enhanced Input)
│   └── Player/          # BP_ThirdPersonCharacter / Controller
├── UI/                  # Widgets UMG
│   ├── ChatBot/         # WBP_ChatBot_Final
│   ├── Dialogs/
│   ├── Minigames/       # WBP_MG_NetworkCabling
│   └── Puzzles/         # WBP_Puzzle_IF
├── Interaction/
│   ├── Puzzles/         # DoorIF, NetworkGame
│   └── Terminals/       # ChatBot
├── Gameplay/            # BP interactivos + Minigames
├── World/               # Islas, Blueprints, Materials, Meshes
├── Maps/                # Firts.umap (nivel principal)
├── Data/ChatBot/        # DT_FAQ_IS, FS_ChatBotFAQ
├── Assets/              # Meshes/Materials Sci-Fi importados
└── Characters/          # Mannequins UE5
```

## Flujo de trabajo diario

```bash
git add Content/ Config/ Docs/
git commit -m "feat: descripción del cambio"
git push
```

## Solución de problemas

| Problema | Solución |
|---|---|
| Plugin no compila | Instalar VS2022 con workload `Game Development with C++` |
| Assets LFS no descargaron | `git lfs install` → `git lfs pull` |
| MCP no conecta | Ver `Docs/` (guía de setup MCP) |