## 1) UI flow + interaction logic (screen-to-screen)

Think of this product as a **3-mode app** with **2 global layers**:

* **Mode layer (persistent):** Smart / Manual / Settings (left vertical rail).
* **State layer (contextual):** Setup → Toasting (running) → Completed.
* **Overlay layer (modal):** “My favourites” pops on top of whatever screen you’re on.

### A. Global navigation (always available on main screens)

1. User can tap **Smart / Manual / Settings** on the left rail at any time (except when a modal blocks input).
2. Switching mode changes only the **main content area**; the rail stays in place.

### B. Smart mode flow (color/level driven)

**Smart Setup (S1 / S2)**

1. User selects which slot they are editing: **tap bread “L” or “R”**.
2. User adjusts toast level (a “browning slider” concept).
3. Optional: user chooses a “profile/avatar” preset (carousel row) to instantly load a saved toast level.
4. Optional: user taps **heart** → opens **My favourites modal** to assign/save this toast level to a chosen avatar.
5. User taps **Start** → enters Toasting (Running).

**Toasting Running (S4)**
6. System shows per-slot progress (percentage). Inactive/empty slot shows a gray placeholder.
7. User taps **Cancel** → stops toasting → returns to the previous Setup screen (Smart or Manual, whichever started it).

**Completed (S5)**
8. When done, UI shows “Completed” plus 100% tiles.
9. User can tap the **heart** on each slot tile to save that result to favourites (either direct toggle or opens modal, depending how you implement).

### C. Manual mode flow (program driven)

**Manual Setup (S6)**

1. User selects slot L/R.
2. User chooses a **program icon** (small circular buttons).
3. User chooses **Fresh / Frozen** state.
4. Optional: Reset.
5. Start → goes to Toasting Running (same S4) → Completed (same S5).

### D. Settings flow (S7)

* Settings is a grid/list of big tiles:

  * **Manage toaster slots**
  * **Change language**

### E. Favourites modal behavior (S3)

* Opens from:

  * Heart button (setup screen)
  * Potentially heart on completed tile
  * Pencil/edit icon in the profile carousel
* Modal blocks background input; background dims.
* Close paths:

  * X (top-right)
  * Cancel button
  * Confirm button (when an avatar is selected)

---

## 2) Detailed description of each UI screen (what to build in LVGL)

### Screen S1 — Smart Setup (basic “toast level”)

**Purpose:** Set desired browning level for L/R and start.

**Layout**

* **Left rail (vertical)**

  * 3 items: Smart (selected), Manual, Settings
  * Icon + label; selected state uses orange highlight.
* **Main area (right)**

  1. **Toaster top visualization**

     * Two bread tiles sitting in a toaster slot well:

       * Left: “L”
       * Right: “R”
     * Selected slot is **orange/browned**, unselected is gray.
  2. **Toast level slider row**

     * Long horizontal track (light gray).
     * Filled portion in warm gradient (orange → pale).
     * Round knob with tiny label showing which slot you’re editing (e.g. “L”).
  3. **Bottom actions**

     * Left: heart icon button (favourites)
     * Center: large orange pill **Start**

**Interactions**

* Tap bread “L/R” → changes active slot, slider now edits that slot’s level.
* Drag slider → updates target level for active slot and updates bread color.
* Tap heart → opens S3 (My favourites).
* Tap Start → goes to S4 (Toasting Running).

**LVGL mapping**

* Rail: `lv_obj` container + three `lv_btn` (or `lv_obj` clickable rows).
* Bread tiles: `lv_btn` with custom style (rounded rect + gradient) + label.
* Slider: `lv_slider` with custom knob + left-to-right gradient on indicator.
* Start: `lv_btn` big pill; Heart: small `lv_btn` with icon.

---

### Screen S2 — Smart Setup (with profile carousel / preset row)

**Purpose:** Same as S1, but adds quick presets via “avatars” and edit.

**What’s different vs S1**

* Under the bread area, there’s a **rounded capsule panel** (like a “preset dock”):

  * Left arrow (previous)
  * 2–3 circular avatar icons (selectable)
  * A vertical divider
  * A **pencil/edit** icon (manage favourites)
  * Right arrow (next)
* Bread tiles can show a **small avatar badge** overlay (top-right corner of bread), indicating which profile is applied to that slot.

**Interactions**

* Tap an avatar icon → loads that avatar’s saved toast level into the active slot (updates slider + bread color).
* Tap arrows → scroll carousel.
* Tap pencil → opens S3 modal (manage/select avatar) — your implementation can decide whether pencil opens “manage list” vs “assign current level”.
* Heart behavior same as S1.

**LVGL mapping**

* Carousel dock: `lv_obj` rounded container with:

  * arrow buttons (`lv_btn`)
  * avatar buttons (`lv_btn` with circular style; `lv_img` inside)
  * divider (`lv_obj` thin line)
  * pencil button (`lv_btn`)
* If you need actual scrolling: `lv_obj` + `LV_FLEX_FLOW_ROW` + manual paging, or `lv_tileview`/custom list.

---

### Screen S3 — Modal: “My favourites” (Cancel variant + Confirm variant)

**Purpose:** Assign/save a toast level to an avatar (profile), or pick an avatar symbol.

**Modal structure**

* Background dims (the underlying screen is visible but darkened).
* Center modal card with rounded corners.
* Top row:

  * Title: **My favourites**
  * Close icon **X** (top-right)
* Content area split:

  * **Left panel**: big circle avatar preview + label “Toasting level”
  * **Right panel**: text “Select your desired symbol” + a grid of avatar icons
* Bottom action button:

  * Variant A (no selection): **Cancel** (gray)
  * Variant B (selection made): **Confirm** (orange)

**Interactions**

* Tap avatar in grid → highlights selection (orange outline/fill), enables Confirm.
* Tap Confirm → saves mapping `{selected_avatar → current_toast_level (+ optional bread type/fresh-frozen)}` and closes modal, returning to the previous screen.
* Tap Cancel or X → closes without saving.

**LVGL mapping**

* Dim layer: full-screen `lv_obj` with semi-transparent bg, clickable to block input.
* Modal card: centered `lv_obj` with shadow + radius.
* Avatar grid: `lv_btnmatrix` (nice for fixed grids) OR a flex container with circular `lv_btn`s.
* Confirm/Cancel: one button whose style/text changes based on selection state.

---

### Screen S4 — Toasting Running (single-slot or dual-slot progress)

**Purpose:** Show live progress; allow cancel.

**Layout**

* Top: **Cancel** pill button centered.
* Main: one or two large rounded-square tiles:

  * Active slot tile:

    * “Toasting”
    * Big percentage (e.g., 60%, 58%)
    * Optional small avatar badge and/or heart bubble (as shown)
  * Inactive/empty slot tile:

    * Gray tile with “R” (or “L”) placeholder

**Interactions**

* Cancel → stop and return to previous setup screen (Smart or Manual).
* If you keep heart visible while running: tapping heart can mark “save this result when finished” (optional).

**LVGL mapping**

* Two tiles: two `lv_obj` cards with labels; update percentage label from backend event.
* If single-slot active: show second tile in disabled style.

---

### Screen S5 — Completed

**Purpose:** Confirm done state; allow saving result to favourites.

**Layout**

* Top: “Completed” label in a pill.
* Main: two tiles (or one active + one disabled), each showing:

  * avatar icon (inside tile)
  * “Toasting 100%”
  * a small **heart bubble** at tile corner (save/favourite)

**Interactions**

* Tap heart bubble on a tile:

  * Option 1: toggles favourite instantly for current profile
  * Option 2 (closer to what the UI suggests): opens S3 modal so user chooses which avatar to save under
* After a timeout or user action (not shown): can return to setup.

**LVGL mapping**

* Same tile components as S4, but fixed at 100% and with “Completed” header.

---

### Screen S6 — Manual Setup

**Purpose:** Choose a program + Fresh/Frozen and start.

**Layout**

* Left rail: Manual highlighted.
* Top bread tiles: L/R selection same as Smart.
* Mid row: a sequence of small round **program buttons** (icons).
* Fresh/Frozen toggle:

  * Two small pills; selected is orange (Fresh in screenshot), unselected gray.
* Left: **Reset** small button.
* Bottom: big **Start** button.

**Interactions**

* Tap program icon → selects program (highlight).
* Tap Fresh/Frozen → toggles state.
* Reset → returns manual selections to default.
* Start → goes to S4.

**LVGL mapping**

* Program icons row: flex row of circular `lv_btn` + `lv_img`.
* Fresh/Frozen: two `lv_btn` in a segmented-control container.
* Keep consistent styles with Smart.

---

### Screen S7 — Settings

**Purpose:** Minimal appliance settings entry points.

**Layout**

* Left rail: Settings highlighted.
* Main: big card tiles (rounded, shadow):

  * “Manage toaster slots” (swap icon)
  * “Change language” (A icon)

**Interactions**

* Tap a tile → push into sub-screen (not provided in your image set).
* Back handling: either a back button or use rail to leave settings.

**LVGL mapping**

* Two large `lv_btn` tiles in a grid/flex row.

## LVGL implementation notes (style + structure that makes it look like this)

If your goal is visual similarity, these matter more than exact pixels:

* **Rounded corners everywhere**

  * Cards / tiles / buttons all share a common radius.
* **Soft shadows**

  * LVGL `shadow_width`, `shadow_opa`, small `shadow_ofs_y`.
* **Warm accent gradient**

  * Bread + progress tiles aren’t a flat orange; they’re gradient.
* **Disabled states**

  * Inactive slot uses low-contrast gray + “L/R” letter.
* **Consistent hierarchy**

  * Rail = navigation
  * Top = status (Cancel/Completed)
  * Center = primary info (tiles/breads)
  * Bottom = primary action (Start)