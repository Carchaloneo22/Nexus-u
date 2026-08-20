# GUÍA VISUAL - Interactividad Dinámica del Widget Puzzle IF

## FLUJO COMPLETO DEL USUARIO

```
USUARIO ABRE WIDGET
       │
       ├──→ Ver nodos: Iniciar → Branch IF → True/False → Abrir/Cerrar
       ├──→ Ver ComboBox: "La puerta está cerrada" (default)
       ├──→ Ver botón: [Verificar] (activo)
       ├──→ Ver botón: [Abrir Puerta] (GRIS/DESHABILITADO)
       └──→ Ver botón: [Salir] (activo)
       
       USUARIO CAMBIA COMBOBOX?
       ├── NO → Sigue "cerrada"
       └── SI → Cambia a "abierta"
       
       USUARIO CLICKEA [VERIFICAR]
       │
       ├──→ Widget lee valor del ComboBox
       ├──→ Compara con "La puerta está cerrada"
       │
       ├──→ SI ES "CERRADA" (CORRECTO):
       │     ├──→ TXT_Resultado cambia a: "¡CORRECTO! Condición TRUE"
       │     ├──→ TXT_Resultado cambia a COLOR AMARILLO/VERDE
       │     ├──→ Rama True se ILUMINA (Border verde)
       │     ├──→ Rama False se APAGA (Border gris)
       │     ├──→ Flecha True se hace VISIBLE/BRILLANTE
       │     ├──→ Flecha False se OSCURECE
       │     └──→ BTN_AbrirPuerta se HABILITA (ya no gris)
       │
       └──→ SI ES "ABIERTA" (INCORRECTO):
             ├──→ TXT_Resultado cambia a: "Condición FALSE. Intenta de nuevo."
             ├──→ TXT_Resultado cambia a COLOR ROJO
             ├──→ Rama True se APAGA (Border gris)
             ├──→ Rama False se ILUMINA (Border rojo)
             ├──→ Flecha True se OSCURECE
             ├──→ Flecha False se hace VISIBLE
             └──→ BTN_AbrirPuerta permanece DESHABILITADO
       
       USUARIO CLICKEA [ABRIR PUERTA] (solo si está habilitado)
       │
       ├──→ Se dispara Event Dispatcher "OnPuzzleSolved"
       ├──→ TXT_Resultado: "¡Puerta abierta! Cruza el puente."
       ├──→ Espera 2 segundos
       └──→ Widget se CIERRA (Remove from Parent)
       
       AFUERA (en el mundo 3D):
       ├──→ Puerta rota 90 grados (se abre)
       ├──→ Puente aparece (escala de 0 a 1)
       └──→ Jugador puede cruzar a Isla 2
```

---

## PASO A PASO - EVENT GRAPH (Nodos exactos)

### 1. EVENT CONSTRUCT (Inicialización al abrir widget)

```
[Event Construct]
    │
    ├──→ [Set Is Enabled]
    │     Target: BTN_AbrirPuerta
    │     Enabled: FALSE (desmarcado)
    │
    ├──→ [Set Text]
    │     Target: TXT_Resultado
    │     Text: "Selecciona la condición y presiona Verificar"
    │
    ├──→ [Set Color and Opacity]  (OPCIONAL)
    │     Target: Border_RamaTrue
    │     Color: Gris oscuro (R=0.3, G=0.3, B=0.3)
    │
    └──→ [Set Color and Opacity]  (OPCIONAL)
          Target: Border_RamaFalse
          Color: Gris oscuro (R=0.3, G=0.3, B=0.3)
```

**¿Cómo hacer Set Color and Opacity en Blueprint?**
1. Arrastra Border_RamaTrue al graph
2. Arrastra pin → busca "Set Color and Opacity"
3. En el pin "In Color", click derecho → "Make Linear Color"
4. Valores RGB (0-1): R=0.3, G=0.3, B=0.3 para gris

---

### 2. ON CLICKED BTN_VERIFY (Lógica principal)

```
[On Clicked (BTN_Verify)]   ← Arrastra BTN_Verify desde Variables al graph
    │
    ├──→ [Get Selected Option]
    │     Target: CMB_Condicion
    │     Return Value (String) ───────┐
    │                                  │
    └──→ [Equal (String)]              │
          A: Return Value ─────────────┘
          B: "La puerta está cerrada"   ← Escribe esto en el campo
          Return Value (Bool) ───→ [Branch]
                                        │
                          ┌───────────────┴───────────────┐
                          │ TRUE                          │ FALSE
                          │ (puerta cerrada)              │ (puerta abierta)
                          │                               │
                          ↓                               ↓
```

**RAMA TRUE (Correcto):**

```
[Branch - True]
    │
    ├──→ [Set Text]
    │     Target: TXT_Resultado
    │     Text: "¡CORRECTO! Condición TRUE → Abrir Puerta habilitado."
    │
    ├──→ [Set Color and Opacity]
    │     Target: TXT_Resultado
    │     Color: AMARILLO (R=1, G=0.8, B=0) o VERDE (R=0.2, G=1, B=0.2)
    │
    ├──→ [Set Is Enabled]
    │     Target: BTN_AbrirPuerta
    │     Enabled: TRUE (marcado)
    │
    ├──→ [Set Color and Opacity]  ← Resaltar rama True
    │     Target: Border_RamaTrue (o como se llame tu borde de True)
    │     Color: VERDE (R=0.1, G=0.6, B=0.1)
    │
    ├──→ [Set Color and Opacity]  ← Apagar rama False
    │     Target: Border_RamaFalse
    │     Color: GRIS (R=0.3, G=0.3, B=0.3)
    │
    ├──→ [Set Visibility]  ← Opcional si tienes flechas como Image
    │     Target: IMG_FlechaTrue
    │     Visibility: Visible
    │
    └──→ [Set Visibility]
          Target: IMG_FlechaFalse
          Visibility: Hidden
```

**RAMA FALSE (Incorrecto):**

```
[Branch - False]
    │
    ├──→ [Set Text]
    │     Target: TXT_Resultado
    │     Text: "Condición FALSE. Selecciona 'La puerta está cerrada'."
    │
    ├──→ [Set Color and Opacity]
    │     Target: TXT_Resultado
    │     Color: ROJO (R=1, G=0.2, B=0.2)
    │
    ├──→ [Set Is Enabled]
    │     Target: BTN_AbrirPuerta
    │     Enabled: FALSE (desmarcado)
    │
    ├──→ [Set Color and Opacity]  ← Apagar rama True
    │     Target: Border_RamaTrue
    │     Color: GRIS (R=0.3, G=0.3, B=0.3)
    │
    ├──→ [Set Color and Opacity]  ← Resaltar rama False
    │     Target: Border_RamaFalse
    │     Color: ROJO (R=0.6, G=0.1, B=0.1)
    │
    ├──→ [Set Visibility]
    │     Target: IMG_FlechaTrue
    │     Visibility: Hidden
    │
    └──→ [Set Visibility]
          Target: IMG_FlechaFalse
          Visibility: Visible
```

---

### 3. ON CLICKED BTN_AbrirPuerta (Al ganar)

```
[On Clicked (BTN_AbrirPuerta)]   ← Arrastra BTN_AbrirPuerta al graph
    │
    ├──→ [Call OnPuzzleSolved]   ← Arrastra tu Event Dispatcher desde Variables
    │     Target: Self
    │
    ├──→ [Set Text]
    │     Target: TXT_Resultado
    │     Text: "¡PUERTA ABIERTA! Cruza el puente hacia Isla 2."
    │
    ├──→ [Set Color and Opacity]
    │     Target: TXT_Resultado
    │     Color: VERDE BRILLANTE (R=0, G=1, B=0)
    │
    ├──→ [Delay]
    │     Duration: 2.0
    │
    └──→ [Remove from Parent]
          Target: Self
```

---

### 4. ON CLICKED BTN_Cerrar (Salir sin resolver)

```
[On Clicked (BTN_Cerrar)]   ← Ya tienes esto con CloseWidget
    │
    └──→ [Remove from Parent]
          Target: Self
```

---

## NODOS NECESARIOS RESUMEN

| Nodo | Cómo encontrarlo | Para qué sirve |
|---|---|---|
| **Get Selected Option** | Arrastrar pin de ComboBox | Leer texto seleccionado |
| **Equal (String)** | Click derecho → "==" o "equal" | Comparar strings |
| **Branch** | Click derecho → "branch" | If/Then/Else |
| **Set Is Enabled** | Arrastrar pin de botón | Habilitar/deshabilitar botón |
| **Set Text** | Arrastrar pin de TextBlock | Cambiar texto |
| **Set Color and Opacity** | Arrastrar pin de Border/Text | Cambiar color visual |
| **Set Visibility** | Arrastrar pin de Image | Mostrar/ocultar flechas |
| **Delay** | Click derecho → "delay" | Esperar X segundos |
| **Remove from Parent** | Click derecho → "remove from parent" | Cerrar widget |
| **Call OnPuzzleSolved** | Arrastrar Event Dispatcher | Notificar al mundo exterior |

---

## PALETA DE COLORES RECOMENDADA (Linear Color 0-1)

| Estado | R | G | B | A |
|---|---|---|---|---|
| Éxito/Verde | 0.2 | 0.8 | 0.2 | 1.0 |
| Error/Rojo | 1.0 | 0.2 | 0.2 | 1.0 |
| Advertencia/Amarillo | 1.0 | 0.8 | 0.0 | 1.0 |
| Inactivo/Gris | 0.3 | 0.3 | 0.3 | 1.0 |
| Morado nodo | 0.4 | 0.1 | 0.5 | 1.0 |
| Azul nodo | 0.1 | 0.3 | 0.6 | 1.0 |

**Cómo crear color:** Click derecho → "Make Linear Color" → rellena R, G, B

---

## ¿CÓMO SABER SI FUNCIONA?

### Test en el Editor (PIE - Play In Editor):

1. Presiona **Play** (▶) en el Editor
2. Acércate al terminal y presiona **Interact** (E o tu tecla asignada)
3. Widget debería abrirse
4. **No toques** el ComboBox (deja "cerrada")
5. Clickea **Verificar**:
   - ✅ Debe aparecer texto verde/amarillo: "¡CORRECTO!"
   - ✅ Botón "Abrir Puerta" debe activarse (ya no gris)
   - ✅ Rama True debería ponerse verde (si pusiste Set Color)
6. Clickea **Abrir Puerta**:
   - ✅ Widget cierra después de 2 segundos
   - ✅ En el mundo 3D: puerta rota, puente aparece
7. Vuelve a abrir widget, cambia a "abierta", clickea Verificar:
   - ✅ Debe aparecer texto rojo: "Condición FALSE"
   - ✅ Botón "Abrir Puerta" debe seguir gris/deshabilitado

---

## ERRORES COMUNES Y SOLUCIONES

| Error | Causa | Solución |
|---|---|---|
| "Accessed None" | Referencia a widget que no existe | Asegurar que el widget esté nombrado correctamente en Designer |
| Botón no se habilita | Set Is Enabled conectado mal | Verificar que el pin "Enabled" está marcado TRUE en rama True |
| ComboBox lee vacío | No hay default option | En Designer, poner "Selected Option" = "La puerta está cerrada" |
| Colores no cambian | Set Color and Opacity conectado a tipo incorrecto | Solo funciona en Borders, TextBlocks, Images. No en Buttons |
| Event Dispatcher no dispara | No está bindeado en BP_PuzzleTerminal_IF | Ir al Blueprint del terminal y hacer Bind |

---

## PROXIMO PASO: CONECTAR CON EL MUNDO 3D

Cuando esto funcione, falta:

### En BP_PuzzleTerminal_IF:

```
Event Interact (o tu evento de interaccion)
    │
    ├──→ [Create Widget]
    │     Class: WBP_Puzzle_IF
    │     Return Value: Guardar en variable "ActiveWidget"
    │
    ├──→ [Add to Viewport]
    │     Target: ActiveWidget
    │
    ├──→ [Set Input Mode UI Only]
    │     In Widget to Focus: ActiveWidget
    │
    ├──→ [Show Mouse Cursor]
    │     Target: Get Player Controller → True
    │
    └──→ [Bind Event to OnPuzzleSolved]
          Target: ActiveWidget
          Event: [Custom Event "HandlePuzzleComplete"]
                │
                ├──→ [Set Input Mode Game Only]
                │
                ├──→ [Show Mouse Cursor]
                │     Target: Get Player Controller → False
                │
                ├──→ [Set Actor Rotation]
                │     Target: PuertaRef (variable tipo Actor)
                │     New Rotation: 0, 90, 0
                │
                ├──→ [Set Actor Scale 3D]
                │     Target: PuenteRef (variable tipo Actor)
                │     New Scale: 4, 1.5, 1
                │
                └──→ [Set ActiveWidget] = None
```

**Variables que necesitas en BP_PuzzleTerminal_IF:**
- `PuertaRef` (tipo: Actor, o mejor: tu BP_Puerta_IF específico)
- `PuenteRef` (tipo: Actor, o tu BP_Puente_IF)
- `ActiveWidget` (tipo: WBP_Puzzle_IF Object Reference)

---

¿Necesitas que te explique con más detalle algún nodo específico del Blueprint?
