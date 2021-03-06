/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

let tabViewShownCount = 0;

// ----------
function test() {
  waitForExplicitFinish();

  // verify initial state
  ok(!TabView.isVisible(), "Tab View starts hidden");

  // launch tab view for the first time
  window.addEventListener("tabviewshown", onTabViewLoadedAndShown, false);
  let tabViewCommand = document.getElementById("Browser:ToggleTabView");
  tabViewCommand.doCommand();
}

// ----------
function onTabViewLoadedAndShown() {
  window.removeEventListener("tabviewshown", onTabViewLoadedAndShown, false);

  // Evidently sometimes isVisible (which is based on the selectedIndex of the
  // tabview deck) isn't updated immediately when called from button.doCommand,
  // so we add a little timeout here to get outside of the doCommand call.
  // If the initial timeout isn't enough, we keep waiting in case it's taking
  // longer than expected.
  // See bug 594909.
  let deck = document.getElementById("tab-view-deck");
  function waitForSwitch() {
    if (deck.selectedIndex == 1) {
      ok(TabView.isVisible(), "Tab View is visible. Count: " + tabViewShownCount);
      tabViewShownCount++;

      // kick off the series
      window.addEventListener("tabviewshown", onTabViewShown, false);
      window.addEventListener("tabviewhidden", onTabViewHidden, false);
      TabView.toggle();
    } else {
      setTimeout(waitForSwitch, 10);
    }
  }

  setTimeout(waitForSwitch, 1);
}

// ----------
function onTabViewShown() {
  // add the count to the message so we can track things more easily.
  ok(TabView.isVisible(), "Tab View is visible. Count: " + tabViewShownCount);
  tabViewShownCount++;
  TabView.toggle();
}

// ----------
function onTabViewHidden() {
  ok(!TabView.isVisible(), "Tab View is hidden. Count: " + tabViewShownCount);

  if (tabViewShownCount == 1) {
    document.getElementById("menu_tabview").doCommand();
  } else if (tabViewShownCount == 2) {
    EventUtils.synthesizeKey("e", { accelKey: true, shiftKey: true });
  } else if (tabViewShownCount == 3) {
    window.removeEventListener("tabviewshown", onTabViewShown, false);
    window.removeEventListener("tabviewhidden", onTabViewHidden, false);
    finish();
  }
}
