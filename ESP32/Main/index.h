#ifndef INDEX_H
#define INDEX_H

const char REMOTE_HTML[] PROGMEM = R"TRINETRAHTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
<meta name="theme-color" content="#040d12">
<meta name="color-scheme" content="dark">
<meta name="mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
<meta name="apple-mobile-web-app-title" content="Trinetra">
<title>TRINETRA // REMOTE CONSOLE</title>
<link rel="icon" href="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 32 32' fill='none' stroke='%234df5a8' stroke-width='1.9' stroke-linecap='round' stroke-linejoin='round'%3E%3Cpath d='M2 21c3.6-5.2 8.3-7.8 14-7.8S26.4 15.8 30 21c-3.6 5.2-8.3 7.8-14 7.8S5.6 26.2 2 21z'/%3E%3Ccircle cx='16' cy='21' r='4.2'/%3E%3Cpath d='M16 1.8c3 3 3 7.2 0 10.4-3-3.2-3-7.4 0-10.4z'/%3E%3C/svg%3E">
<style>
/* ==========================================================================
   TRINETRA // REMOTE CONSOLE
   Glass-cockpit controller. One file, no external fonts or libraries,
   so it loads instantly even on the robot's own Wi-Fi (no internet).
   ========================================================================== */
:root{
  /* base */
  --abyss:#040d12; --cell:#09161d; --cell-hi:#112530;
  --ice:#e8f3f7; --ice2:#b9cdd5; --mut:#7d99a4; --dim:#5a7683;
  /* signal colours as r,g,b triplets (used as rgb(var(--x)) / rgba(var(--x),.4)) */
  --steel:134,180,198; --ph:77,245,168; --amber:255,176,32; --red:255,77,94; --optic:85,204,242; --violet:167,139,250;
  --hair:rgba(var(--steel),.16); --hair2:rgba(var(--steel),.34);
  /* type: instrument-style condensed sans for readouts, system UI for sentences */
  --inst:"Bahnschrift","DIN Alternate","DIN Condensed","Roboto Condensed","sans-serif-condensed","Arial Narrow",system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;
  --ui:system-ui,-apple-system,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif;
  --ease:cubic-bezier(.2,.8,.2,1);
  --vh:1vh; --gap:8px; --ic:19px; --fs-k:8.5px; --fs-l:10px; --fs-v:20px;
  --pad:clamp(116px,min(calc(var(--vh)*100 - 156px),24vw),240px);
}
@supports (height:100dvh){:root{--vh:1dvh}}

*{box-sizing:border-box;margin:0;padding:0;-webkit-tap-highlight-color:transparent;-webkit-touch-callout:none;-webkit-user-select:none;user-select:none}
[hidden]{display:none!important}
html{height:100%;background:var(--abyss);color-scheme:dark;overscroll-behavior:none;-webkit-text-size-adjust:100%;text-size-adjust:100%}
body{height:100%;overflow:hidden;color:var(--ice);font:500 12px/1.25 var(--inst);font-variant-numeric:tabular-nums;touch-action:manipulation;-webkit-font-smoothing:antialiased}
button{font:inherit;color:inherit;background:none;border:0;cursor:pointer}
a{color:inherit;text-decoration:none}
input{-webkit-user-select:text;user-select:text}
button:focus-visible,a:focus-visible,input:focus-visible{outline:2px solid rgb(var(--optic));outline-offset:2px}
.ic{width:var(--ic);height:var(--ic);flex:none;display:block;fill:none;stroke:currentColor;stroke-width:1.7;stroke-linecap:round;stroke-linejoin:round}

/* ---------- backdrop ---------- */
.bg{position:fixed;inset:0;z-index:0;pointer-events:none;
  background:
    radial-gradient(900px 520px at 50% 40%,rgba(var(--ph),.07),transparent 70%),
    radial-gradient(700px 420px at 100% 100%,rgba(var(--optic),.05),transparent 65%),
    linear-gradient(180deg,#071820 0%,var(--abyss) 62%)}
.bg::after{content:"";position:absolute;inset:0;background:radial-gradient(ellipse at center,transparent 55%,rgba(0,0,0,.55) 100%)}

/* ---------- layout ---------- */
.app{position:relative;z-index:1;height:calc(var(--vh)*100);max-width:1560px;margin:0 auto;display:grid;grid-template-columns:minmax(0,1fr);grid-template-rows:auto minmax(0,1fr) auto;gap:var(--gap);
  padding:max(8px,env(safe-area-inset-top)) max(12px,env(safe-area-inset-right)) max(8px,env(safe-area-inset-bottom)) max(12px,env(safe-area-inset-left))}
.stage{display:grid;gap:var(--gap);min-height:0;grid-template-columns:auto minmax(0,1fr) clamp(180px,25vw,300px);grid-template-areas:"drive center keys"}
.drive{grid-area:drive}.center{grid-area:center}.keys{grid-area:keys}
.center{display:grid;grid-template-rows:auto minmax(0,1fr);gap:var(--gap);min-height:0;min-width:0}

/* ---------- top bar ---------- */
.top{display:flex;align-items:center;gap:8px;min-height:38px;padding:0 2px 6px;border-bottom:1px solid var(--hair)}
.brand{display:flex;align-items:center;gap:10px;margin-right:auto;min-width:0;overflow:hidden}
.logo{width:28px;height:28px;flex:none;fill:none;stroke:rgb(var(--ph));stroke-width:1.5;stroke-linecap:round;stroke-linejoin:round;filter:drop-shadow(0 0 6px rgba(var(--ph),.5))}
.logo circle[fill]{fill:rgb(var(--ph))}
.brand-t{display:grid;gap:4px;min-width:0}
.brand-t b{font:300 15px/1 var(--inst);letter-spacing:.42em;color:var(--ice);white-space:nowrap}
.brand-t i{font:600 7.5px/1 var(--inst);font-style:normal;letter-spacing:.3em;color:var(--mut);white-space:nowrap}

.ann{display:inline-flex;align-items:center;gap:7px;height:26px;padding:0 9px;border-radius:6px;border:1px solid var(--hair2);background:rgba(0,0,0,.28);
  font:700 10px/1 var(--inst);letter-spacing:.12em;color:var(--ice2);white-space:nowrap;transition:background .2s,border-color .2s,color .2s,box-shadow .2s}
.ann .led{width:7px;height:7px;border-radius:50%;flex:none;background:rgb(var(--c,var(--ph)));box-shadow:0 0 8px rgba(var(--c,var(--ph)),.8)}
.ann .ic{width:14px;height:14px}
.link[data-s=connecting]{--c:var(--amber)}
.link[data-s=lost]{--c:var(--red);color:#ffc2c9;border-color:rgba(var(--red),.6);animation:ann 1.1s ease-in-out infinite}
.link[data-s=sim]{--c:var(--violet);color:#e3d8ff;border-color:rgba(var(--violet),.55)}
.bars{display:inline-flex;align-items:flex-end;gap:2px;height:12px}
.bars i{width:3px;border-radius:1px;background:currentColor;opacity:.22}
.bars i:nth-child(1){height:4px}.bars i:nth-child(2){height:7px}.bars i:nth-child(3){height:10px}.bars i:nth-child(4){height:12px}
.bars[data-n="1"] i:nth-child(-n+1),.bars[data-n="2"] i:nth-child(-n+2),.bars[data-n="3"] i:nth-child(-n+3),.bars[data-n="4"] i{opacity:1}
.ms{min-width:3.2ch;text-align:right;color:var(--mut);font-weight:600}
.tgt{color:var(--mut)}
.tgt[data-s=clear] .led{display:none}
.tgt[data-s=target]{--c:var(--amber);color:rgb(var(--amber));border-color:rgba(var(--amber),.7);background:rgba(var(--amber),.12);animation:ann 1.1s ease-in-out infinite}
.tgt[data-s=locked]{color:#fff;border-color:rgb(var(--red));background:rgba(var(--red),.8);box-shadow:0 0 18px rgba(var(--red),.5)}

.ibtn{display:grid;place-items:center;height:30px;min-width:32px;padding:0 8px;border-radius:8px;border:1px solid var(--hair);background:rgba(255,255,255,.03);color:var(--ice2);transition:background .15s,border-color .15s}
.ibtn .ic{width:16px;height:16px}
.ibtn:hover,.ibtn.press{background:var(--cell-hi);border-color:var(--hair2)}
.stop{display:inline-flex;align-items:center;gap:6px;height:30px;padding:0 11px;border-radius:8px;border:1px solid rgba(var(--red),.55);background:rgba(var(--red),.12);color:#ffb9c1;font:700 10.5px/1 var(--inst);letter-spacing:.14em;transition:background .12s}
.stop .ic{width:15px;height:15px}
.stop:hover{background:rgba(var(--red),.2)}.stop.press{background:rgba(var(--red),.42)}

/* ---------- telemetry strip ---------- */
.tele{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:1px;border:1px solid var(--hair);border-radius:14px;overflow:hidden;background:var(--hair)}
.cell{position:relative;min-width:0;padding:8px 10px 14px;background:var(--cell)}
.cell .k{display:flex;align-items:baseline;justify-content:space-between;gap:6px;font:600 var(--fs-k)/1 var(--inst);letter-spacing:.16em;color:var(--mut)}
.cell .k em{font-style:normal;font-weight:600;letter-spacing:.06em;color:var(--dim)}
.cell .v{display:block;margin-top:6px;font:600 var(--fs-v)/1 var(--inst);color:var(--ice);white-space:nowrap;overflow:hidden;text-overflow:ellipsis;transition:color .2s}
.cell .v small{margin-left:2px;font-size:.55em;font-weight:600;color:var(--mut);letter-spacing:.04em}
.cell.drv .v{font-size:calc(var(--fs-v)*.68);padding-top:.2em;letter-spacing:.04em}
.meter{position:absolute;left:10px;right:10px;bottom:6px;height:2px;border-radius:2px;background:rgba(var(--steel),.13);overflow:hidden}
.meter b{display:block;height:100%;width:0;background:rgb(var(--ph));transition:width .12s linear,background .2s}
.cell[data-lvl=warn] .v{color:rgb(var(--amber))}.cell[data-lvl=warn] .meter b{background:rgb(var(--amber))}
.cell[data-lvl=bad] .v{color:rgb(var(--red))}.cell[data-lvl=bad] .meter b{background:rgb(var(--red))}
.cell[data-lvl=bad]{animation:cellwarn .7s ease-in-out infinite alternate}
.app[data-link=lost] .cell .v{opacity:.35}

/* ---------- scope panel ---------- */
.scope{display:flex;flex-direction:column;min-height:0;overflow:hidden;border:1px solid var(--hair);border-radius:18px;
  background:radial-gradient(110% 100% at 50% 100%,#0b2a2c 0%,#07171d 55%,#050f14 100%);box-shadow:inset 0 1px 0 rgba(255,255,255,.05),0 22px 44px -24px #000}
.scope-head{display:flex;align-items:center;justify-content:space-between;gap:8px;padding:6px 8px;border-bottom:1px solid var(--hair)}
.scope-body{position:relative;flex:1;min-height:0}
#radar{position:absolute;inset:0;width:100%;height:100%;display:block;transition:opacity .3s}
.app[data-link=lost] #radar{opacity:.3}

.tabs{position:relative;display:inline-grid;grid-template-columns:1fr 1fr;padding:2px;border-radius:9px;background:rgba(0,0,0,.38);border:1px solid var(--hair)}
.tab{position:relative;z-index:1;display:flex;align-items:center;justify-content:center;gap:6px;padding:6px 12px;border-radius:7px;font:700 var(--fs-l)/1 var(--inst);letter-spacing:.12em;color:var(--mut);transition:color .2s}
.tab .ic{width:14px;height:14px}
.tab[aria-selected=true]{color:var(--ice)}
.thumb{position:absolute;z-index:0;top:2px;bottom:2px;left:2px;width:calc(50% - 2px);border-radius:7px;background:var(--cell-hi);box-shadow:inset 0 1px 0 rgba(255,255,255,.08),0 0 0 1px var(--hair2);transition:transform .3s var(--ease)}
.thumb::after{content:"";position:absolute;left:24%;right:24%;bottom:0;height:2px;border-radius:2px;background:rgb(var(--ph));box-shadow:0 0 8px rgba(var(--ph),.8);transition:background .3s,box-shadow .3s}
.scope[data-view=cam] .thumb{transform:translateX(100%)}
.scope[data-view=cam] .thumb::after{background:rgb(var(--optic));box-shadow:0 0 8px rgba(var(--optic),.8)}

.chip{display:inline-flex;align-items:center;gap:7px;height:24px;max-width:50%;padding:0 9px;border-radius:6px;border:1px solid var(--hair2);background:rgba(0,0,0,.28);
  font:700 9.5px/1 var(--inst);letter-spacing:.14em;color:var(--ice2);white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.chip i{width:6px;height:6px;border-radius:50%;flex:none;background:rgb(var(--c,var(--ph)));box-shadow:0 0 7px rgba(var(--c,var(--ph)),.8)}
.chip[data-s=hold]{--c:var(--steel)}.chip[data-s=wait]{--c:var(--amber)}.chip[data-s=cam]{--c:var(--optic)}
.chip[data-s=lost]{--c:var(--red);color:#ffc2c9}
.chip[data-s=locked]{--c:var(--red);color:#ffc2c9;border-color:rgba(var(--red),.6)}

.hint{position:absolute;z-index:5;left:50%;bottom:12px;transform:translateX(-50%);max-width:92%;padding:9px 14px;border-radius:8px;border:1px solid rgba(var(--violet),.6);background:rgba(20,14,40,.88);
  color:#e3d8ff;font:600 10px/1.2 var(--inst);letter-spacing:.1em;text-transform:uppercase;text-align:center}

/* ---------- camera ---------- */
.cam{position:absolute;inset:0;display:none;background:#02070a}
.scope[data-view=cam] .cam{display:block}
.scope[data-view=cam] #radar{display:none}
.cam iframe,.cam img{position:absolute;inset:0;width:100%;height:100%;object-fit:contain;border:0;background:#000}
.cam-state{position:absolute;inset:0;z-index:2;display:grid;place-content:center;justify-items:center;gap:10px;background:#030b0f;color:var(--ice2);text-align:center;padding:12px;transition:opacity .4s,visibility .4s}
.cam-state.hide{opacity:0;visibility:hidden;pointer-events:none}
.cam-state b{font:700 10px/1 var(--inst);letter-spacing:.18em}
.cam-state small{font:400 11.5px/1.3 var(--ui);color:var(--mut)}
.spin{width:22px;height:22px;border-radius:50%;border:2px solid rgba(var(--optic),.25);border-top-color:rgb(var(--optic));animation:spin .9s linear infinite}
.cam-state.fail .spin{display:none}
.brackets{position:absolute;inset:10px;z-index:1;pointer-events:none;opacity:.8;
  background:
    linear-gradient(rgb(var(--optic)),rgb(var(--optic))) left top/16px 1.5px no-repeat,linear-gradient(rgb(var(--optic)),rgb(var(--optic))) left top/1.5px 16px no-repeat,
    linear-gradient(rgb(var(--optic)),rgb(var(--optic))) right top/16px 1.5px no-repeat,linear-gradient(rgb(var(--optic)),rgb(var(--optic))) right top/1.5px 16px no-repeat,
    linear-gradient(rgb(var(--optic)),rgb(var(--optic))) left bottom/16px 1.5px no-repeat,linear-gradient(rgb(var(--optic)),rgb(var(--optic))) left bottom/1.5px 16px no-repeat,
    linear-gradient(rgb(var(--optic)),rgb(var(--optic))) right bottom/16px 1.5px no-repeat,linear-gradient(rgb(var(--optic)),rgb(var(--optic))) right bottom/1.5px 16px no-repeat}
.cam-tools{position:absolute;z-index:4;top:12px;right:12px;display:flex;gap:6px}
.cam-tools .ibtn{background:rgba(3,10,14,.62);-webkit-backdrop-filter:blur(6px);backdrop-filter:blur(6px)}
.fpvbtn{font:700 9.5px/1 var(--inst);letter-spacing:.12em}
.fpvbtn span{display:inline-flex;align-items:center;gap:6px}
.fpvbtn .out,body.fpv .fpvbtn .in{display:none}
body.fpv .fpvbtn .out{display:inline-flex}
.pip{display:none;position:absolute;z-index:3;top:54px;right:12px;width:clamp(130px,22vw,210px);height:clamp(76px,12.5vw,120px);border-radius:12px;overflow:hidden;border:1px solid var(--hair2);background:rgba(3,10,14,.62);-webkit-backdrop-filter:blur(8px);backdrop-filter:blur(8px)}
.pip canvas{display:block;width:100%;height:100%}

/* ---------- drive pad ---------- */
.drive{position:relative;display:flex;flex-direction:column;align-items:center;justify-content:center;gap:12px;padding:0 4px}
.drive::before{content:"";position:absolute;left:50%;top:50%;width:calc(var(--pad)*1.6);height:calc(var(--pad)*1.6);transform:translate(-50%,-56%);background:radial-gradient(circle,rgba(var(--ph),.08),transparent 62%);pointer-events:none}
.drive-state{position:relative;font:700 var(--fs-l)/1 var(--inst);letter-spacing:.2em;color:var(--mut);white-space:nowrap;transition:color .15s}
.drive-state[data-on="1"]{color:rgb(var(--ph));text-shadow:0 0 12px rgba(var(--ph),.7)}
.pad{position:relative;width:var(--pad);height:var(--pad);border-radius:50%;touch-action:none;cursor:grab;
  background:radial-gradient(circle at 50% 36%,rgba(var(--steel),.1),rgba(3,10,13,.94) 70%),#030b0e;border:1px solid var(--hair2);
  box-shadow:inset 0 0 0 7px rgba(var(--steel),.03),inset 0 6px 30px rgba(0,0,0,.7),0 14px 30px -12px #000;transition:box-shadow .2s,border-color .2s}
.pad.drag{cursor:grabbing;border-color:rgba(var(--ph),.65);box-shadow:inset 0 0 0 7px rgba(var(--ph),.05),inset 0 6px 30px rgba(0,0,0,.7),0 0 36px rgba(var(--ph),.2)}
.ticks{position:absolute;inset:6px;border-radius:50%;opacity:.7;pointer-events:none;
  background:repeating-conic-gradient(from -.6deg,rgba(var(--steel),.55) 0 1.2deg,transparent 1.2deg 7.5deg);
  -webkit-mask:radial-gradient(closest-side,transparent calc(100% - 8px),#000 calc(100% - 7px));mask:radial-gradient(closest-side,transparent calc(100% - 8px),#000 calc(100% - 7px))}
.wedge{position:absolute;inset:0;border-radius:50%;opacity:0;pointer-events:none;transition:opacity .12s;
  -webkit-mask:radial-gradient(closest-side,transparent 32%,#000 33%);mask:radial-gradient(closest-side,transparent 32%,#000 33%)}
.pad[data-dir=F] .wedge{opacity:1;background:conic-gradient(from -45deg,rgba(var(--ph),.26) 0 90deg,transparent 90deg)}
.pad[data-dir=R] .wedge{opacity:1;background:conic-gradient(from 45deg,rgba(var(--ph),.26) 0 90deg,transparent 90deg)}
.pad[data-dir=B] .wedge{opacity:1;background:conic-gradient(from 135deg,rgba(var(--ph),.26) 0 90deg,transparent 90deg)}
.pad[data-dir=L] .wedge{opacity:1;background:conic-gradient(from 225deg,rgba(var(--ph),.26) 0 90deg,transparent 90deg)}
.gate{position:absolute;left:50%;top:50%;width:46%;height:46%;transform:translate(-50%,-50%);border-radius:50%;border:1px dashed rgba(var(--steel),.22);pointer-events:none}
.chev{position:absolute;display:grid;place-items:center;width:20%;height:20%;color:rgba(var(--steel),.4);transition:color .12s,filter .12s;pointer-events:none}
.chev .ic{width:62%;height:62%;stroke-width:2.2}
.chev.up{left:40%;top:9%}.chev.down{left:40%;bottom:9%;transform:rotate(180deg)}.chev.left{left:9%;top:40%;transform:rotate(-90deg)}.chev.right{right:9%;top:40%;transform:rotate(90deg)}
.pad[data-dir=F] .up,.pad[data-dir=B] .down,.pad[data-dir=L] .left,.pad[data-dir=R] .right{color:rgb(var(--ph));filter:drop-shadow(0 0 7px rgba(var(--ph),.9))}
.pad[data-warn=warn] .up{color:rgb(var(--amber))}
.pad[data-warn=bad] .up{color:rgb(var(--red));animation:blink .5s steps(2) infinite}
.knob{position:absolute;left:50%;top:50%;width:40%;height:40%;margin:-20% 0 0 -20%;border-radius:50%;display:grid;place-items:center;will-change:transform;
  background:radial-gradient(circle at 34% 24%,#6d8793 0,#2e424c 36%,#101b21 100%);
  box-shadow:inset 0 1px 1px rgba(255,255,255,.4),inset 0 -9px 14px rgba(0,0,0,.55),0 12px 20px rgba(0,0,0,.65),0 0 0 1px rgba(var(--steel),.4);
  transition:transform .26s cubic-bezier(.2,1.6,.35,1),box-shadow .2s}
.knob::before{content:"";position:absolute;inset:17%;border-radius:50%;border:1.5px solid rgba(var(--ph),.85);box-shadow:0 0 12px rgba(var(--ph),.5),inset 0 0 10px rgba(var(--ph),.3);transition:box-shadow .2s}
.knob .ic{position:relative;width:46%;height:46%;stroke:rgb(var(--ph));stroke-width:1.5;filter:drop-shadow(0 0 4px rgba(var(--ph),.7))}
.knob .ic circle[fill]{fill:rgb(var(--ph))}
.pad.drag .knob{transition:box-shadow .2s;box-shadow:inset 0 1px 1px rgba(255,255,255,.4),inset 0 -9px 14px rgba(0,0,0,.55),0 12px 20px rgba(0,0,0,.65),0 0 0 1px rgba(var(--ph),.6),0 0 26px rgba(var(--ph),.3)}
.pad.drag .knob::before{box-shadow:0 0 20px rgba(var(--ph),.85),inset 0 0 14px rgba(var(--ph),.5)}

/* ---------- key bank ---------- */
.keys{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));grid-auto-rows:minmax(0,1fr);gap:1px;overflow:hidden;background:var(--hair);border:1px solid var(--hair);border-radius:16px;box-shadow:0 22px 44px -26px #000}
.key{position:relative;display:flex;flex-direction:column;align-items:center;justify-content:center;gap:6px;min-width:0;min-height:0;padding:6px 4px;background:var(--cell);color:rgb(var(--c,var(--steel)));transition:background .14s var(--ease),opacity .2s}
.key .ic{transition:transform .12s var(--ease)}
.key span{max-width:100%;font:600 var(--fs-l)/1.05 var(--inst);letter-spacing:.1em;text-transform:uppercase;text-align:center;color:var(--ice2)}
.key::before{content:"";position:absolute;top:0;left:50%;width:34%;height:2px;margin-left:-17%;border-radius:0 0 2px 2px;background:rgba(var(--steel),.14);transition:background .2s,box-shadow .2s}
.key.on::before{background:rgb(var(--c));box-shadow:0 0 10px rgba(var(--c),.9)}
.key:hover{background:#0c1d26}.key.press{background:var(--cell-hi)}.key.press .ic{transform:scale(.88)}
.key[aria-disabled=true]{opacity:.4}
.k-horn{--c:var(--amber)}.k-demo{--c:var(--violet)}.k-radar{--c:var(--ph)}.k-radar-off{--c:var(--steel)}
.k-lock{--c:var(--optic)}.k-unlock{--c:var(--steel)}.k-arm{--c:var(--amber)}.k-fire{--c:var(--red)}.k-off{--c:var(--steel)}
.k-lock.hot{--c:var(--red);background:rgba(var(--red),.14)}
.k-lock.cue{--c:var(--amber);animation:keycue 1s ease-in-out infinite alternate}
.k-demo.run::after,.k-arm .drain{content:"";position:absolute;left:0;right:0;bottom:0;height:2px;transform-origin:left;transform:scaleX(0)}
.k-demo.run::after{background:rgb(var(--violet));animation:fill var(--demo,180s) linear forwards}
.k-arm .drain{background:rgb(var(--amber))}
.row{grid-column:1/-1;display:grid;grid-template-columns:1fr 1.25fr 1fr;gap:1px;min-height:0;background:var(--hair)}
.row[data-guard="0"]{grid-template-columns:1.25fr 1fr}
.row[data-guard="0"] .k-arm{display:none}
.row[data-state=armed] .k-arm::before{background:rgb(var(--amber));box-shadow:0 0 10px rgba(var(--amber),.9)}
.row[data-state=armed] .drain{animation:drain var(--arm,6s) linear forwards}
.row[data-state=armed] .k-fire{--c:var(--amber);background:repeating-linear-gradient(135deg,rgba(var(--amber),.16) 0 7px,rgba(var(--amber),.05) 7px 14px)}
.row[data-state=armed] .k-fire::before{background:rgb(var(--amber));box-shadow:0 0 10px rgba(var(--amber),.9)}
.row[data-state=on] .k-fire{background:rgba(var(--red),.2);animation:keycue .5s ease-in-out infinite alternate}
.row[data-state=on] .k-fire::before{background:rgb(var(--red));box-shadow:0 0 12px rgba(var(--red),1)}

/* ---------- footer ---------- */
.foot{display:flex;align-items:center;justify-content:space-between;gap:12px;padding:6px 2px 0;border-top:1px solid var(--hair);font:600 8.5px/1 var(--inst);letter-spacing:.18em;color:var(--dim)}
.foot b{color:var(--mut);font-weight:700}
.foot a{display:inline-flex;align-items:center;gap:6px;color:var(--mut)}
.foot a .ic{width:11px;height:11px}
.foot a:hover{color:var(--ice)}

/* ---------- settings sheet ---------- */
.sheet{position:fixed;inset:0;z-index:120;display:grid;place-items:center;padding:12px;visibility:hidden;opacity:0;transition:opacity .22s,visibility .22s}
.sheet.open{visibility:visible;opacity:1}
.scrim{position:absolute;inset:0;background:rgba(2,8,11,.74);-webkit-backdrop-filter:blur(6px);backdrop-filter:blur(6px)}
.card{position:relative;width:min(540px,100%);max-height:calc(var(--vh)*100 - 24px);display:flex;flex-direction:column;border-radius:20px;border:1px solid var(--hair2);background:#0a171e;box-shadow:0 30px 80px -20px #000;transform:translateY(14px) scale(.985);transition:transform .3s var(--ease)}
.sheet.open .card{transform:none}
.card-head{display:flex;align-items:center;justify-content:space-between;gap:12px;padding:14px 16px;border-bottom:1px solid var(--hair)}
.card-head b{display:block;font:600 16px/1.1 var(--ui);color:var(--ice)}
.card-head small{display:block;margin-top:4px;font:400 12px/1.2 var(--ui);color:var(--mut)}
.card-body{overflow:auto;-webkit-overflow-scrolling:touch;overscroll-behavior:contain;touch-action:pan-y;padding:4px 16px 18px}
.grp-t{margin:18px 0 4px;font:700 9.5px/1 var(--inst);letter-spacing:.2em;color:var(--mut)}
.opt{display:flex;align-items:center;justify-content:space-between;gap:14px;padding:12px 0;border-bottom:1px solid var(--hair)}
.opt b{display:block;font:600 14px/1.2 var(--ui);color:var(--ice)}
.opt small{display:block;margin-top:3px;font:400 12px/1.35 var(--ui);color:var(--mut)}
.sw{position:relative;flex:none;width:46px;height:28px;border-radius:99px;background:rgba(var(--steel),.18);border:1px solid var(--hair2);transition:background .2s,border-color .2s}
.sw i{position:absolute;top:2px;left:2px;width:22px;height:22px;border-radius:50%;background:#cfe0e6;box-shadow:0 2px 6px rgba(0,0,0,.5);transition:transform .25s var(--ease),background .2s}
.sw[aria-checked=true]{background:rgba(var(--ph),.26);border-color:rgba(var(--ph),.7)}
.sw[aria-checked=true] i{transform:translateX(18px);background:rgb(var(--ph))}
.field{display:block;margin-top:10px}
.field span{display:block;margin-bottom:6px;font:400 12px/1.2 var(--ui);color:var(--mut)}
.field input{width:100%;height:42px;padding:0 12px;border-radius:10px;border:1px solid var(--hair2);background:rgba(0,0,0,.35);color:var(--ice);font:500 16px/1 var(--ui)}
.btns{display:flex;flex-wrap:wrap;gap:8px;margin-top:10px}
.sbtn{display:inline-flex;align-items:center;justify-content:center;gap:8px;height:40px;padding:0 16px;border-radius:10px;border:1px solid var(--hair2);background:rgba(255,255,255,.04);color:var(--ice);font:600 13px/1 var(--ui)}
.sbtn.pri{background:rgba(var(--ph),.16);border-color:rgba(var(--ph),.6);color:#d9ffee}
.sbtn .ic{width:15px;height:15px}
.kbd-help{display:grid;gap:8px;margin-top:8px;font:400 12.5px/1.3 var(--ui);color:var(--ice2)}
.kbd-help div{display:flex;align-items:center;gap:5px}
.kbd-help span{margin-left:auto;color:var(--mut)}
kbd{min-width:24px;padding:4px 7px;border-radius:6px;border:1px solid var(--hair2);border-bottom-width:2px;background:rgba(255,255,255,.04);font:600 11px/1 var(--inst);color:var(--ice);text-align:center}

/* ---------- toasts ---------- */
#toasts{position:fixed;left:50%;bottom:calc(14px + env(safe-area-inset-bottom));transform:translateX(-50%);z-index:150;display:flex;flex-direction:column;align-items:center;gap:6px;pointer-events:none;width:max-content;max-width:92vw}
.toast{max-width:100%;padding:9px 15px;border-radius:10px;border:1px solid var(--hair2);background:rgba(5,16,21,.94);box-shadow:0 10px 28px rgba(0,0,0,.6);
  font:500 12.5px/1.25 var(--ui);color:var(--ice);animation:tin .28s var(--ease) both;transition:opacity .35s,transform .35s}
.toast.warn{border-color:rgba(var(--amber),.6);color:#ffe3a6}
.toast.err{border-color:rgba(var(--red),.65);color:#ffc7cf}
.toast.sim{border-color:rgba(var(--violet),.6);color:#e3d8ff}
.toast.out{opacity:0;transform:translateY(8px)}

/* ---------- boot ---------- */
#boot{position:fixed;inset:0;z-index:200;display:grid;place-items:center;background:radial-gradient(circle at 50% 46%,#08242a 0,#030c10 70%);transition:opacity .55s ease,visibility .55s}
#boot.done{opacity:0;visibility:hidden;pointer-events:none}
.boot-in{display:grid;justify-items:center;gap:18px}
.boot-logo{width:96px;height:96px;fill:none;stroke:rgb(var(--ph));stroke-width:1.1;stroke-linecap:round;stroke-linejoin:round;filter:drop-shadow(0 0 16px rgba(var(--ph),.55))}
.boot-logo .s{stroke-dasharray:120;stroke-dashoffset:120;animation:draw 1s var(--ease) forwards}
.boot-logo .s2{animation-delay:.18s}.boot-logo .s3{animation-delay:.34s}
.boot-logo .dot{fill:rgb(var(--ph));stroke:none;opacity:0;animation:pop .4s .9s forwards}
.boot-name{font:300 24px/1 var(--inst);letter-spacing:.55em;text-indent:.55em;color:var(--ice);opacity:0;animation:rise .6s .55s var(--ease) forwards}
.boot-line{width:150px;height:1px;background:rgba(var(--steel),.2);overflow:hidden}
.boot-line i{display:block;height:100%;background:rgb(var(--ph));transform:scaleX(0);transform-origin:left;animation:fill 1s .2s var(--ease) forwards}

/* ---------- keyframes ---------- */
@keyframes spin{to{transform:rotate(1turn)}}
@keyframes blink{50%{opacity:.25}}
@keyframes ann{50%{box-shadow:0 0 14px rgba(var(--c,var(--amber)),.55)}}
@keyframes cellwarn{to{background:#1b1318}}
@keyframes keycue{to{box-shadow:inset 0 0 18px rgba(var(--c),.35)}}
@keyframes fill{from{transform:scaleX(0)}to{transform:scaleX(1)}}
@keyframes drain{from{transform:scaleX(1)}to{transform:scaleX(0)}}
@keyframes draw{to{stroke-dashoffset:0}}
@keyframes rise{from{opacity:0;transform:translateY(6px)}to{opacity:1;transform:none}}
@keyframes pop{to{opacity:1}}
@keyframes tin{from{opacity:0;transform:translateY(10px) scale(.96)}}

/* ---------- FPV: full-bleed camera with the controls floating on top ---------- */
body.fpv .foot{display:none}
body.fpv .top{position:relative;z-index:60;padding:3px 8px;border:0;border-radius:12px;background:linear-gradient(180deg,rgba(2,8,11,.82),rgba(2,8,11,.25));-webkit-backdrop-filter:blur(8px);backdrop-filter:blur(8px)}
body.fpv .scope{position:fixed;inset:0;z-index:30;border:0;border-radius:0;background:#000;box-shadow:none}
body.fpv .scope-head{display:none}
body.fpv .scope-body{position:absolute;inset:0}
body.fpv{--pad:clamp(104px,min(34vh,36vw),170px)}
body.fpv .brand-t,body.fpv .brackets{display:none}
body.fpv .cam-tools{top:calc(max(8px,env(safe-area-inset-top)) + 52px);right:max(12px,env(safe-area-inset-right))}
body.fpv .pip{display:block;top:calc(max(8px,env(safe-area-inset-top)) + 92px);right:max(12px,env(safe-area-inset-right))}
body.fpv .tele{position:fixed;z-index:50;left:max(12px,env(safe-area-inset-left));top:calc(max(8px,env(safe-area-inset-top)) + 52px);grid-template-columns:repeat(2,86px);border-radius:12px;-webkit-backdrop-filter:blur(8px);backdrop-filter:blur(8px)}
body.fpv .cell{background:rgba(4,12,16,.6);padding:6px 8px 12px}
body.fpv .cell .v{font-size:16px;margin-top:5px}
body.fpv .cell.drv .v{font-size:11px}
body.fpv .drive{position:fixed;z-index:50;left:max(14px,env(safe-area-inset-left));bottom:max(12px,env(safe-area-inset-bottom));padding:0;gap:8px}
body.fpv .drive::before{display:none}
body.fpv .pad{background:radial-gradient(circle at 50% 36%,rgba(var(--steel),.14),rgba(3,10,13,.62) 70%);-webkit-backdrop-filter:blur(6px);backdrop-filter:blur(6px)}
body.fpv .drive-state{text-shadow:0 1px 6px #000}
body.fpv .keys{position:fixed;z-index:50;right:max(12px,env(safe-area-inset-right));bottom:max(12px,env(safe-area-inset-bottom));width:min(250px,48vw);grid-auto-rows:42px;background:rgba(var(--steel),.22);-webkit-backdrop-filter:blur(10px);backdrop-filter:blur(10px)}
body.fpv .key{background:rgba(4,12,16,.58);gap:4px}
body.fpv .key .ic{width:17px;height:17px}
body.fpv .key span{font-size:9px}
body.fpv #toasts{bottom:auto;top:calc(max(8px,env(safe-area-inset-top)) + 56px)}

/* ---------- portrait phones: scope on top, pad + keys at the thumbs ---------- */
@media (orientation:portrait){
  body.fpv #toasts{top:36%}
  :root{--pad:clamp(124px,min(38vw,25vh),204px)}
  .stage{grid-template-columns:auto minmax(0,1fr);grid-template-rows:minmax(0,1fr) auto;grid-template-areas:"center center" "drive keys"}
  .keys{grid-auto-rows:clamp(48px,7.4vh,64px)}
  .brand-t i,#btnFs,.foot-mid,.link #linkTxt,.link .bars,.stop span{display:none}
  .top{gap:6px}
  .brand{gap:8px}
  .brand-t b{font-size:13px;letter-spacing:.28em}
  .logo{width:26px;height:26px}
  .ann{padding:0 8px;gap:6px}
  .stop{padding:0;width:34px;justify-content:center}
  .drive{padding:4px 2px}
}

/* ---------- short landscape phones ---------- */
@media (orientation:landscape) and (max-width:760px){
  .tab .ic{display:none}
  .tab{padding:6px 10px}
}
@media (orientation:landscape) and (max-height:430px){
  .foot{display:none}
  .cell{padding:6px 9px 13px}
  .top{min-height:34px;padding-bottom:5px}
}

/* ---------- laptops and tablets ---------- */
@media (min-width:1000px) and (min-height:600px){
  :root{--gap:12px;--ic:23px;--fs-k:10px;--fs-l:11.5px;--fs-v:30px}
  .top{min-height:44px;gap:10px;padding-bottom:10px}
  .logo{width:32px;height:32px}
  .brand-t b{font-size:17px}
  .cell{padding:12px 14px 18px}
  .meter{left:14px;right:14px;bottom:8px}
  .tab{padding:8px 16px}
  .key{gap:10px}
  .key .ic{width:30px;height:30px}
  .key span{font-size:12.5px}
  .ann,.stop,.ibtn{height:34px}
  .foot{font-size:10px;padding-top:10px}
}

@media (prefers-reduced-motion:reduce){
  .boot-logo .s,.boot-logo .dot,.boot-name,.boot-line i{animation-duration:.01ms;animation-delay:0s}
  .ann,.link,.tgt,.pad .chev,.cell,.key{animation:none!important}
  .toast{animation:none}
}
</style>
</head>
<body>
<svg width="0" height="0" style="position:absolute" aria-hidden="true" focusable="false"><defs>
<symbol id="i-eye" viewBox="0 0 32 32"><path d="M2 21c3.6-5.2 8.3-7.8 14-7.8S26.4 15.8 30 21c-3.6 5.2-8.3 7.8-14 7.8S5.6 26.2 2 21z"/><circle cx="16" cy="21" r="4.2"/><circle cx="16" cy="21" r="1.4" fill="currentColor" stroke="none"/><path d="M16 1.8c3 3 3 7.2 0 10.4-3-3.2-3-7.4 0-10.4z"/></symbol>
<symbol id="i-horn" viewBox="0 0 24 24"><path d="M11 5 6.5 9H3v6h3.5L11 19V5z"/><path d="M15.5 8.5a5 5 0 0 1 0 7"/><path d="M18.6 5.4a9.3 9.3 0 0 1 0 13.2"/></symbol>
<symbol id="i-bolt" viewBox="0 0 24 24"><path d="M13 2.5 4.5 13.5H11l-1 8 8.5-11H12l1-8z"/></symbol>
<symbol id="i-radar" viewBox="0 0 24 24"><circle cx="12" cy="12" r="9"/><circle cx="12" cy="12" r="4.8"/><path d="m12 12 6.2-6.2"/><circle cx="12" cy="12" r="1" fill="currentColor" stroke="none"/></symbol>
<symbol id="i-radar-off" viewBox="0 0 24 24"><circle cx="12" cy="12" r="9"/><circle cx="12" cy="12" r="4.8"/><path d="M5.6 5.6l12.8 12.8"/></symbol>
<symbol id="i-target" viewBox="0 0 24 24"><circle cx="12" cy="12" r="7.5"/><path d="M12 2.5v4.5M12 17v4.5M2.5 12H7M17 12h4.5"/><circle cx="12" cy="12" r="1.3" fill="currentColor" stroke="none"/></symbol>
<symbol id="i-unlock" viewBox="0 0 24 24"><rect x="4.5" y="11" width="15" height="10" rx="2.2"/><path d="M8 11V7.5a4 4 0 0 1 7.7-1.5"/></symbol>
<symbol id="i-flame" viewBox="0 0 24 24"><path d="M8.5 14.5A2.5 2.5 0 0 0 11 12c0-1.38-.5-2-1-3-1.07-2.14-.22-4.05 2-6 .5 2.5 2 4.9 4 6.5 2 1.6 3 3.5 3 5.5a7 7 0 1 1-14 0c0-1.15.43-2.29 1-3a2.5 2.5 0 0 0 2.5 2.5z"/></symbol>
<symbol id="i-power" viewBox="0 0 24 24"><path d="M12 3v8.5"/><path d="M17.8 6.6a8.2 8.2 0 1 1-11.6 0"/></symbol>
<symbol id="i-shield" viewBox="0 0 24 24"><path d="M12 21.5s7.5-3.6 7.5-9.5V5.6L12 2.8 4.5 5.6V12c0 5.9 7.5 9.5 7.5 9.5z"/><path d="M12 8.4v4.2"/><circle cx="12" cy="15.6" r=".7" fill="currentColor" stroke="none"/></symbol>
<symbol id="i-camera" viewBox="0 0 24 24"><path d="M14.5 4h-5L8 6.5H5a2 2 0 0 0-2 2V18a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2V8.5a2 2 0 0 0-2-2h-3L14.5 4z"/><circle cx="12" cy="13" r="3.5"/></symbol>
<symbol id="i-sliders" viewBox="0 0 24 24"><path d="M4 7h9M17 7h3M4 12h3M11 12h9M4 17h11M19 17h1"/><circle cx="15" cy="7" r="2"/><circle cx="9" cy="12" r="2"/><circle cx="17" cy="17" r="2"/></symbol>
<symbol id="i-full" viewBox="0 0 24 24"><path d="M8 3.5H5.5a2 2 0 0 0-2 2V8M16 3.5h2.5a2 2 0 0 1 2 2V8M20.5 16v2.5a2 2 0 0 1-2 2H16M8 20.5H5.5a2 2 0 0 1-2-2V16"/></symbol>
<symbol id="i-x" viewBox="0 0 24 24"><path d="M18 6 6 18M6 6l12 12"/></symbol>
<symbol id="i-chev" viewBox="0 0 24 24"><path d="m6.5 14.5 5.5-5.5 5.5 5.5"/></symbol>
<symbol id="i-refresh" viewBox="0 0 24 24"><path d="M20.5 12a8.5 8.5 0 1 1-2.6-6.1L20.5 8"/><path d="M20.5 3.5V8H16"/></symbol>
<symbol id="i-edit" viewBox="0 0 24 24"><path d="M12 20.5h8.5"/><path d="M16.2 3.8a2.1 2.1 0 0 1 3 3L7.5 18.5l-4 1 1-4 11.7-11.7z"/></symbol>
<symbol id="i-expand" viewBox="0 0 24 24"><path d="M14 4.5h5.5V10M10 19.5H4.5V14M19.5 4.5l-6 6M4.5 19.5l6-6"/></symbol>
<symbol id="i-shrink" viewBox="0 0 24 24"><path d="M19.5 10H14V4.5M4.5 14H10v5.5M14 10l6-6M10 14l-6 6"/></symbol>
<symbol id="i-stop" viewBox="0 0 24 24"><path d="M8.2 2.8h7.6l5.4 5.4v7.6l-5.4 5.4H8.2l-5.4-5.4V8.2z"/><rect x="9" y="9" width="6" height="6" rx="1" fill="currentColor" stroke="none"/></symbol>
<symbol id="i-ext" viewBox="0 0 24 24"><path d="M8 16 16.5 7.5M9 7.5h7.5V15"/></symbol>
</defs></svg>

<div class="bg" aria-hidden="true"></div>

<div id="boot" aria-hidden="true">
  <div class="boot-in">
    <svg class="boot-logo" viewBox="0 0 32 32">
      <path class="s" d="M2 21c3.6-5.2 8.3-7.8 14-7.8S26.4 15.8 30 21c-3.6 5.2-8.3 7.8-14 7.8S5.6 26.2 2 21z"/>
      <circle class="s s2" cx="16" cy="21" r="4.2"/>
      <path class="s s3" d="M16 1.8c3 3 3 7.2 0 10.4-3-3.2-3-7.4 0-10.4z"/>
      <circle class="dot" cx="16" cy="21" r="1.4"/>
    </svg>
    <div class="boot-name">TRINETRA</div>
    <div class="boot-line"><i></i></div>
  </div>
</div>

<div class="app" id="app" data-link="connecting">

  <header class="top">
    <div class="brand">
      <svg class="logo" viewBox="0 0 32 32" aria-hidden="true"><use href="#i-eye"/></svg>
      <div class="brand-t"><b>TRINETRA</b><i>REMOTE CONSOLE</i></div>
    </div>
    <div class="ann link" id="link" data-s="connecting" role="status">
      <i class="led"></i><span id="linkTxt">LINKING</span>
      <span class="bars" id="bars" data-n="0"><i></i><i></i><i></i><i></i></span>
      <span class="ms" id="ping">--</span>
    </div>
    <div class="ann tgt" id="targetBadge" data-s="clear" role="status">
      <svg class="ic" aria-hidden="true"><use href="#i-target"/></svg><span id="targetTxt">CLEAR</span>
    </div>
    <button class="stop" data-fast data-act="estop" aria-label="Emergency stop"><svg class="ic" aria-hidden="true"><use href="#i-stop"/></svg><span>STOP</span></button>
    <button class="ibtn" id="btnFs" data-act="fs" aria-label="Toggle full screen"><svg class="ic" aria-hidden="true"><use href="#i-full"/></svg></button>
    <button class="ibtn" data-act="settings" aria-label="Settings"><svg class="ic" aria-hidden="true"><use href="#i-sliders"/></svg></button>
  </header>

  <main class="stage">

    <!-- DRIVE -->
    <section class="drive" aria-label="Drive">
      <div class="pad" id="pad" data-dir="S" aria-label="Drive joystick">
        <i class="ticks"></i><i class="wedge"></i>
        <span class="chev up"><svg class="ic"><use href="#i-chev"/></svg></span>
        <span class="chev right"><svg class="ic"><use href="#i-chev"/></svg></span>
        <span class="chev down"><svg class="ic"><use href="#i-chev"/></svg></span>
        <span class="chev left"><svg class="ic"><use href="#i-chev"/></svg></span>
        <i class="gate"></i>
        <div class="knob" id="knob"><svg class="ic" viewBox="0 0 32 32"><use href="#i-eye"/></svg></div>
      </div>
      <div class="drive-state" id="driveState">DRAG TO DRIVE</div>
    </section>

    <!-- TELEMETRY + SCOPE -->
    <section class="center">
      <div class="tele" aria-label="Telemetry">
        <div class="cell"><span class="k">RADAR</span><span class="v"><span id="tRadar">--</span><small>&deg;</small></span><i class="meter"><b id="mRadar"></b></i></div>
        <div class="cell" id="cDist"><span class="k">DIST<em>cm</em></span><span class="v"><span id="tDist">--</span></span><i class="meter"><b id="mDist"></b></i></div>
        <div class="cell" id="cFront"><span class="k">FRONT<em>cm</em></span><span class="v"><span id="tFront">--</span></span><i class="meter"><b id="mFront"></b></i></div>
        <div class="cell drv"><span class="k">DRIVE</span><span class="v" id="tDrive">STOP</span></div>
      </div>

      <div class="scope" id="vp" data-view="radar">
        <div class="scope-head">
          <div class="tabs" role="tablist">
            <i class="thumb"></i>
            <button class="tab" role="tab" data-act="viewRadar" data-view="radar" aria-selected="true"><svg class="ic" aria-hidden="true"><use href="#i-radar"/></svg>RADAR</button>
            <button class="tab" role="tab" data-act="viewCam" data-view="cam" aria-selected="false"><svg class="ic" aria-hidden="true"><use href="#i-camera"/></svg>CAMERA</button>
          </div>
          <div class="chip" id="chip" data-s="hold"><i></i><span id="chipTxt">SWEEP HOLD</span></div>
        </div>
        <div class="scope-body" id="vpBody">
          <canvas id="radar"></canvas>
          <div class="cam" id="cam">
            <img id="camFrame" alt="" src="about:blank">
            <div class="cam-state" id="camState"><i class="spin"></i><b id="camStateTxt">ACQUIRING FEED</b><small id="camHost"></small></div>
            <div class="brackets"></div>
            <div class="cam-tools">
              <button class="ibtn" data-act="camReload" aria-label="Reload feed"><svg class="ic" aria-hidden="true"><use href="#i-refresh"/></svg></button>
              <button class="ibtn" data-act="camEdit" aria-label="Camera address"><svg class="ic" aria-hidden="true"><use href="#i-edit"/></svg></button>
              <button class="ibtn fpvbtn" data-act="fpv" aria-label="Toggle FPV full screen">
                <span class="in"><svg class="ic" aria-hidden="true"><use href="#i-expand"/></svg>FPV</span>
                <span class="out"><svg class="ic" aria-hidden="true"><use href="#i-shrink"/></svg>EXIT</span>
              </button>
            </div>
            <div class="pip" id="pipWrap"><canvas id="pip"></canvas></div>
          </div>
          <button class="hint" id="simHint" data-act="simOn" hidden>No robot link. Tap to preview the console with simulated data</button>
        </div>
      </div>
    </section>

    <!-- KEY BANK -->
    <section class="keys" id="keys" aria-label="Controls">
      <button class="key k-horn" data-fast data-act="horn"><svg class="ic" aria-hidden="true"><use href="#i-horn"/></svg><span>Horn</span></button>
      <button class="key k-demo" id="kDemo" data-fast data-act="demo"><svg class="ic" aria-hidden="true"><use href="#i-bolt"/></svg><span>3-Min Demo</span></button>
      <button class="key k-radar" id="kRadarOn" data-fast data-act="radarOn"><svg class="ic" aria-hidden="true"><use href="#i-radar"/></svg><span>Radar On</span></button>
      <button class="key k-radar-off" id="kRadarOff" data-fast data-act="radarOff"><svg class="ic" aria-hidden="true"><use href="#i-radar-off"/></svg><span>Radar Off</span></button>
      <button class="key k-lock" id="kLock" data-fast data-act="lock"><svg class="ic" aria-hidden="true"><use href="#i-target"/></svg><span>Lock Target</span></button>
      <button class="key k-unlock" id="kUnlock" data-fast data-act="unlock"><svg class="ic" aria-hidden="true"><use href="#i-unlock"/></svg><span>Unlock</span></button>
      <div class="row" id="fireRow" data-state="safe" data-guard="1">
        <button class="key k-arm" data-fast data-act="arm" aria-pressed="false"><svg class="ic" aria-hidden="true"><use href="#i-shield"/></svg><span>Arm</span><i class="drain"></i></button>
        <button class="key k-fire" data-fast data-act="fireOn" aria-disabled="true"><svg class="ic" aria-hidden="true"><use href="#i-flame"/></svg><span>Relay On</span></button>
        <button class="key k-off" data-fast data-act="fireOff"><svg class="ic" aria-hidden="true"><use href="#i-power"/></svg><span>Relay Off</span></button>
      </div>
    </section>

  </main>

  <footer class="foot">
    <span>TRINETRA TACTICAL OS <b>V6.0</b></span>
    <span class="foot-mid">SESSION <b id="clock">00:00:00</b></span>
    <a href="/radar">LAPTOP RADAR <svg class="ic" aria-hidden="true"><use href="#i-ext"/></svg></a>
  </footer>
</div>

<!-- SETTINGS -->
<div class="sheet" id="sheet" aria-hidden="true">
  <div class="scrim" data-act="closeSheet"></div>
  <div class="card" role="dialog" aria-modal="true" aria-labelledby="sTitle">
    <div class="card-head">
      <div><b id="sTitle">Settings</b><small>Saved on this device</small></div>
      <button class="ibtn" data-act="closeSheet" aria-label="Close settings"><svg class="ic" aria-hidden="true"><use href="#i-x"/></svg></button>
    </div>
    <div class="card-body">
      <div class="grp-t">CAMERA FEED</div>
      <label class="field"><span>Front camera stream (or leave blank for ESP32 proxy)</span>
        <input id="camInputFront" type="url" inputmode="url" autocapitalize="off" autocomplete="off" spellcheck="false" placeholder="http://192.168.4.1:81/stream">
      </label>
      <label class="field"><span>Back camera stream (or leave blank for ESP32 proxy)</span>
        <input id="camInputBack" type="url" inputmode="url" autocapitalize="off" autocomplete="off" spellcheck="false" placeholder="http://192.168.4.1:81/stream">
      </label>
      <p style="font:500 10px/1.3 var(--ui);color:var(--mut);margin:4px 0 8px">Both blank = use robot proxy. FRONT/BACK toggle tells ESP32 which upstream to open. Direct URLs bypass the proxy.</p>
      <div class="btns">
        <button class="sbtn pri" data-act="camSave">Save and reload</button>
        <button class="sbtn" data-act="camReset">Reset to default</button>
      </div>

      <div class="grp-t">PREFERENCES</div>
      <div class="opt"><div><b>Sound effects</b><small>Horn, lock-on and alert tones from this device</small></div><button class="sw" role="switch" data-pref="sound" aria-label="Sound effects"><i></i></button></div>
      <div class="opt"><div><b>Haptic feedback</b><small>Short vibrations on taps and alerts (Android)</small></div><button class="sw" role="switch" data-pref="haptics" aria-label="Haptic feedback"><i></i></button></div>
      <div class="opt"><div><b>Fire safety guard</b><small>Tap ARM before FIRE ON works. ARM turns itself off after a few seconds</small></div><button class="sw" role="switch" data-pref="guard" aria-label="Fire safety guard"><i></i></button></div>
      <div class="opt"><div><b>Mirror radar</b><small>Flip left and right if the sweep looks reversed</small></div><button class="sw" role="switch" data-pref="mirror" aria-label="Mirror radar"><i></i></button></div>
      <div class="opt"><div><b>Preview mode</b><small>Run the console with simulated data. Nothing is sent to the robot</small></div><button class="sw" role="switch" data-pref="sim" aria-label="Preview mode"><i></i></button></div>

      <div class="grp-t">TOOLS</div>
      <div class="btns"><a class="sbtn" href="/radar">Open laptop radar <svg class="ic" aria-hidden="true"><use href="#i-ext"/></svg></a></div>

      <div class="grp-t" id="kbdT" hidden>KEYBOARD</div>
      <div class="kbd-help" id="kbdHelp" hidden>
        <div><kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd> or arrow keys<span>Drive while held</span></div>
        <div><kbd>Space</kbd><span>Emergency stop</span></div>
        <div><kbd>H</kbd><span>Horn</span></div>
        <div><kbd>L</kbd><kbd>U</kbd><span>Lock, unlock</span></div>
        <div><kbd>R</kbd><span>Radar on or off</span></div>
        <div><kbd>C</kbd><kbd>V</kbd><span>Camera, FPV view</span></div>
      </div>
    </div>
  </div>
</div>

<div id="toasts" aria-live="polite"></div>
<script>
(function(){
'use strict';

/* =====================  SETTINGS YOU MAY WANT TO TWEAK  ===================== */
const CFG={
  pollMs:100,                       // how often telemetry (/data) is requested
  maxRange:250,                     // radar outer ring, in cm
  echoMax:249,                      // distance above this = "no echo" (same rule as before)
  frontNone:999,                    // FRONT value that means "no reading"
  armMs:6000,                       // fire guard switches itself back to SAFE after this long
  demoMs:180000,                    // length of the demo progress bar (3 minutes)
  camDefault:'http://192.168.4.1:81/stream', // ESP32 proxy (active cam selected via /camera?src=)
  camProxy:'http://192.168.4.1:81/stream'
};

/* ---------- helpers ---------- */
const $=s=>document.querySelector(s),$$=s=>Array.from(document.querySelectorAll(s));
const clamp=(v,a,b)=>Math.min(b,Math.max(a,v));
const num=v=>(v===undefined||v===null||v===''||!isFinite(+v))?NaN:+v;
const tnow=()=>performance.now();
const el={};
['app','link','linkTxt','bars','ping','targetBadge','targetTxt','btnFs','pad','knob','driveState','tRadar','tDist','tFront','tDrive','mRadar','mDist','mFront','cDist','cFront','vp','chip','chipTxt','radar','cam','camFrame','camState','camStateTxt','camHost','pipWrap','pip','simHint','fireRow','sheet','camInputFront','camInputBack','toasts','boot','clock','kbdHelp','kbdT','kDemo','kRadarOn','kRadarOff','kLock','kUnlock']
  .forEach(id=>{el[id]=document.getElementById(id)});

/* ---------- saved preferences ---------- */
const LS={get(k,d){try{const v=localStorage.getItem(k);return v===null?d:v}catch(e){return d}},set(k,v){try{localStorage.setItem(k,v)}catch(e){}}};
const prefs={sound:true,haptics:true,guard:true,mirror:false,sim:false};
try{Object.assign(prefs,JSON.parse(LS.get('trinetra_prefs','{}')))}catch(e){}
prefs.sim=/[?&]sim\b/.test(location.search)||location.protocol==='file:';   // preview mode is never saved
const savePrefs=()=>LS.set('trinetra_prefs',JSON.stringify({sound:prefs.sound,haptics:prefs.haptics,guard:prefs.guard,mirror:prefs.mirror}));

/* ---------- sound + haptics (synthesised on the phone, no audio files) ---------- */
let AC=null,touched=false;                                                     // browsers only allow sound/vibration after the first tap
const audio=()=>{if(!touched)return null;if(!AC){try{AC=new(window.AudioContext||window.webkitAudioContext)()}catch(e){}}if(AC&&AC.state==='suspended')AC.resume();return AC};
function tone(f,d,type,v,at,to){
  if(!prefs.sound)return;const c=audio();if(!c)return;
  const t=c.currentTime+(at||0),o=c.createOscillator(),g=c.createGain();
  o.type=type||'sine';o.frequency.setValueAtTime(f,t);if(to)o.frequency.exponentialRampToValueAtTime(to,t+d);
  g.gain.setValueAtTime(.0001,t);g.gain.exponentialRampToValueAtTime(v||.15,t+.012);g.gain.exponentialRampToValueAtTime(.0001,t+d);
  o.connect(g);g.connect(c.destination);o.start(t);o.stop(t+d+.03);
}
const SFX={
  horn(){tone(600,.4,'square',.2);tone(900,.4,'square',.2,.2)},
  demo(){tone(400,.6,'sawtooth',.2)},
  on(){tone(660,.08,'sine',.12);tone(990,.12,'sine',.12,.09)},
  off(){tone(990,.08,'sine',.1);tone(560,.14,'sine',.1,.09)},
  lock(){tone(1500,.07,'square',.1);tone(1500,.07,'square',.1,.11);tone(2200,.16,'square',.1,.22)},
  unlock(){tone(1300,.09,'triangle',.14);tone(800,.16,'triangle',.14,.1)},
  arm(){tone(520,.07,'square',.09);tone(780,.09,'square',.09,.08)},
  safe(){tone(780,.07,'square',.08);tone(480,.1,'square',.08,.08)},
  fire(){tone(180,.45,'sawtooth',.16,0,60)},
  alert(){tone(900,.12,'square',.1);tone(700,.18,'square',.1,.15)},
  stop(){tone(300,.25,'square',.16)},
  ping(){tone(1700,.06,'sine',.07)}
};
const vib=p=>{if(touched&&prefs.haptics&&navigator.vibrate){try{navigator.vibrate(p)}catch(e){}}};
['pointerdown','keydown'].forEach(n=>addEventListener(n,()=>{touched=true;audio()},{once:true,capture:true}));

/* ---------- toasts ---------- */
function toast(msg,kind){
  const t=document.createElement('div');t.className='toast '+(kind||'');t.textContent=msg;
  el.toasts.appendChild(t);while(el.toasts.children.length>3)el.toasts.firstChild.remove();
  setTimeout(()=>t.classList.add('out'),1700);setTimeout(()=>t.remove(),2150);
}

/* ---------- preview mode: a tiny fake robot so the UI can be tried without hardware ---------- */
const SIM={t:0,a:90,dir:1,run:true,lock:false,want:false,mv:'STOP',last:0};
const WORLD=[{a:56,d:92,w:20},{a:112,d:164,w:26},{a:154,d:218,w:30}];
function simFrame(){
  const n=tnow(),dt=SIM.last?Math.min(.5,(n-SIM.last)/1000):.1;SIM.last=n;SIM.t+=dt;
  if(SIM.run&&!SIM.lock){SIM.a+=SIM.dir*64*dt;if(SIM.a>=168){SIM.a=168;SIM.dir=-1}if(SIM.a<=12){SIM.a=12;SIM.dir=1}}
  let d=255;WORLD.forEach(o=>{if(Math.abs(SIM.a-o.a)<=o.w/2)d=Math.round(o.d+Math.sin(SIM.t*7)*2)});
  const target=d<=CFG.echoMax&&d<190;
  if(SIM.want&&target){SIM.lock=true;SIM.want=false}
  if(SIM.lock&&!target)SIM.lock=false;
  return{angle:Math.round(SIM.a),distance:d,front:Math.round(112+86*Math.sin(SIM.t/2.4)),movement:SIM.mv,locked:SIM.lock,target:target};
}
function simCmd(u){
  const m=u.match(/cmd=(\w)/);
  if(m&&u.indexOf('/move')===0)SIM.mv={F:'FORWARD',B:'BACKWARD',L:'LEFT',R:'RIGHT',S:'STOP'}[m[1]]||'STOP';
  else if(u==='/radar/start')SIM.run=true;
  else if(u==='/radar/stop')SIM.run=false;
  else if(u==='/lock')SIM.want=true;
  else if(u==='/unlock'){SIM.lock=false;SIM.want=false}
  else if(u==='/relay/on'||u==='/fire/on')SIM.relay=true;
  else if(u==='/relay/off'||u==='/fire/off')SIM.relay=false;
}

/* ---------- sending commands ---------- */
function send(url,o){
  o=o||{};
  if(prefs.sim){simCmd(url);if(!o.quiet)toast('Preview: '+url,'sim');return Promise.resolve(true)}
  return fetch(url,{cache:'no-store',keepalive:!!o.keepalive})
    .then(r=>{if(!r.ok)throw 0;return true})
    .catch(()=>{if(!o.quiet)toast('Command not delivered. Check the link.','err');return false});
}

/* ---------- link monitor + telemetry polling ---------- */
let linkState='connecting',prevLink='connecting',fails=0,pingAvg=null,pingDrawn=0,pollT=0;
const LINK_TXT={connecting:'LINKING',online:'ONLINE',lost:'LINK LOST',sim:'PREVIEW'};
function setLink(s,ms){
  if(ms!=null&&(s==='online'||s==='sim'))pingAvg=pingAvg==null?ms:pingAvg*.7+ms*.3;
  if(s!==linkState){
    prevLink=linkState;linkState=s;
    el.app.dataset.link=s;el.link.dataset.s=s;el.linkTxt.textContent=LINK_TXT[s];
    el.simHint.hidden=!(s==='lost'&&!prefs.sim);
    if(s==='lost'){toast('Link lost. The robot is not responding.','err');vib([60,50,60]);SFX.alert()}
    else if(s==='online'&&prevLink==='lost')toast('Link restored');
    updateChip();
  }
  const t=tnow();
  if((s==='online'||s==='sim')&&pingAvg!=null){
    if(t-pingDrawn>400){pingDrawn=t;el.ping.textContent=Math.round(pingAvg)+'ms';el.bars.dataset.n=pingAvg<60?4:pingAvg<120?3:pingAvg<250?2:1}
  }else{el.ping.textContent='--';el.bars.dataset.n=0}
}
async function pollOnce(){
  if(document.hidden){pollT=setTimeout(pollOnce,400);return}
  const t0=tnow(),sim=prefs.sim;
  try{
    let d,ms;
    if(sim){d=simFrame();ms=6+Math.random()*8}
    else{
      const ac=new AbortController(),to=setTimeout(()=>ac.abort(),1800);
      try{const r=await fetch('/data',{cache:'no-store',signal:ac.signal});if(!r.ok)throw new Error('http');d=await r.json()}
      finally{clearTimeout(to)}
      ms=tnow()-t0;
    }
    if(sim!==prefs.sim)throw new Error('mode changed');
    fails=0;setLink(sim?'sim':'online',ms);applyTelemetry(d);
  }catch(e){if(sim===prefs.sim&&++fails>=3)setLink('lost')}
  pollT=setTimeout(pollOnce,fails>=3?900:Math.max(0,CFG.pollMs-(tnow()-t0)));
}
function setSim(v){
  prefs.sim=!!v;fails=0;SIM.last=0;R.blips.length=0;R.init=false;R.last=null;pingAvg=null;
  setLink(v?'sim':'connecting',10);
  toast(v?'Preview mode on. Simulated data, nothing is sent to the robot.':'Preview mode off. Reconnecting to the robot.','sim');
}

/* ---------- telemetry -> screen ---------- */
let tState='clear',lastTap=0;
function setLevel(card,meter,v){
  if(!isFinite(v)){card.dataset.lvl='';meter.style.width='0%';return}
  card.dataset.lvl=v<30?'bad':v<70?'warn':'';
  meter.style.width=clamp((1-v/CFG.maxRange)*100,4,100)+'%';
}
function onTargetState(st){
  el.targetBadge.dataset.s=st;el.targetTxt.textContent={clear:'CLEAR',target:'TARGET',locked:'LOCKED'}[st];
  el.kLock.classList.toggle('hot',st==='locked');el.kLock.classList.toggle('cue',st==='target');el.kLock.classList.toggle('on',st!=='clear');
  const quiet=tnow()-lastTap<900;
  if(st==='locked'){R.lockAt=tnow();if(!quiet){SFX.lock();vib([30,40,30])}}
  else if(st==='target'&&tState==='clear'){SFX.ping();vib(15)}
}
function applyTelemetry(d){
  const t=tnow(),ang=num(d.angle),dist=num(d.distance),fr=num(d.front);
  el.tRadar.textContent=isFinite(ang)?Math.round(ang):'--';
  el.mRadar.style.width=isFinite(ang)?clamp(ang/180*100,0,100)+'%':'0%';
  const dOk=isFinite(dist)&&dist<=CFG.echoMax;
  el.tDist.textContent=dOk?Math.round(dist):'--';setLevel(el.cDist,el.mDist,dOk?dist:NaN);
  const fOk=isFinite(fr)&&fr<CFG.frontNone;
  el.tFront.textContent=fOk?Math.round(fr):'--';setLevel(el.cFront,el.mFront,fOk?fr:NaN);
  el.pad.dataset.warn=fOk?(fr<30?'bad':fr<70?'warn':''):'';
  el.tDrive.textContent=(d.movement==null||d.movement==='')?'STOP':String(d.movement).toUpperCase();
  const st=d.locked?'locked':d.target?'target':'clear';
  if(st!==tState){onTargetState(st);tState=st}
  if(typeof d.relay==='boolean' && d.relay!==fire.on){fire.on=d.relay;if(!d.relay)setArmed(false);renderFire()}
  radarFeed(d,t,ang,dist);
}

/* ---------- radar scope ---------- */
const R={angle:90,disp:90,dir:1,moveAt:0,sweeping:false,locked:false,target:false,blips:[],last:null,init:false,lockAt:0};
const LIFE=4200;
const RAMPS=[
  [[236,255,246],[77,245,168],[10,92,74]],      // normal: white-mint flash -> phosphor green -> deep teal
  [[255,246,220],[255,176,32],[110,64,8]],      // target: amber
  [[255,232,235],[255,77,94],[104,24,38]]       // locked: red
];
function ramp(st,k){
  const r=RAMPS[st];let a,b,t;
  if(k<.22){a=r[0];b=r[1];t=k/.22}else{a=r[1];b=r[2];t=(k-.22)/.78}
  return((a[0]+(b[0]-a[0])*t)|0)+','+((a[1]+(b[1]-a[1])*t)|0)+','+((a[2]+(b[2]-a[2])*t)|0);
}
function radarFeed(d,t,a,dist){
  R.locked=!!d.locked;R.target=!!d.target;
  if(!isFinite(a))return;
  const da=Math.abs(a-R.angle);
  if(!R.init){R.init=true;R.angle=R.disp=a}
  else if(da>=.5){R.dir=a>R.angle?1:-1;R.moveAt=t}
  if(dist>0&&dist<=CFG.echoMax){
    const w=clamp(da/2+1,1.2,5),l=R.blips[R.blips.length-1],st=R.locked?2:R.target?1:0;
    if(l&&Math.abs(l.a-a)<.6&&Math.abs(l.d-dist)<4&&t-l.t<400){l.t=t;l.d=dist;l.st=st}
    else R.blips.push({a:a,d:dist,t:t,w:w,st:st});
    if(R.blips.length>220)R.blips.shift();
    R.last={a:a,d:dist,t:t};
  }
  R.angle=a;
}

function Radar(cv,small){
  const ctx=cv.getContext('2d'),D=Math.PI/180;
  let W=0,H=0,dpr=1,cx=0,cy=0,rad=0,stat=null,ok=false,font='sans-serif',tight=false;
  const ang=a=>-(prefs.mirror?180-a:a)*D;                                    // radar angle -> canvas angle
  const pt=(a,r)=>{const t=-ang(a);return[cx+r*Math.cos(t),cy-r*Math.sin(t)]};

  function build(){
    stat=document.createElement('canvas');stat.width=W;stat.height=H;
    const s=stat.getContext('2d'),S='134,180,198',P='77,245,168';
    const g=s.createRadialGradient(cx,cy,0,cx,cy,rad);
    g.addColorStop(0,'rgba('+P+',.14)');g.addColorStop(.7,'rgba('+P+',.04)');g.addColorStop(1,'rgba('+P+',.02)');
    s.beginPath();s.moveTo(cx,cy);s.arc(cx,cy,rad,Math.PI,2*Math.PI);s.closePath();s.fillStyle=g;s.fill();
    const n=small?3:5;
    for(let i=1;i<=n;i++){
      s.beginPath();s.arc(cx,cy,rad*i/n,Math.PI,2*Math.PI);
      s.strokeStyle='rgba('+S+','+(i===n?.5:.16)+')';s.lineWidth=(i===n?1.4:1)*dpr;s.stroke();
    }
    const step=small?90:30;
    for(let a=0;a<=180;a+=step){
      const p=pt(a,rad);s.beginPath();s.moveTo(cx,cy);s.lineTo(p[0],p[1]);
      s.strokeStyle='rgba('+S+','+(a===90?.26:.12)+')';s.lineWidth=dpr;s.stroke();
    }
    s.beginPath();s.moveTo(cx-rad,cy);s.lineTo(cx+rad,cy);s.strokeStyle='rgba('+S+',.4)';s.lineWidth=dpr;s.stroke();
    if(!small){
      for(let a=0;a<=180;a+=10){
        const L=(a%30?4:8)*dpr,p1=pt(a,rad),p2=pt(a,rad-L);
        s.beginPath();s.moveTo(p1[0],p1[1]);s.lineTo(p2[0],p2[1]);s.strokeStyle='rgba('+S+',.55)';s.lineWidth=dpr;s.stroke();
      }
      s.font='600 '+Math.round(9.5*dpr)+'px '+font;s.textAlign='center';s.textBaseline='middle';s.fillStyle='rgba('+S+',.78)';
      for(let a=0;a<=180;a+=30){const p=pt(a,rad+(tight?11:14)*dpr);s.fillText(a+'\u00B0',p[0],p[1])}
      s.fillStyle='rgba('+S+',.55)';s.textBaseline='top';
      const gap=rad/5/dpr;                                                     // css px between range rings: thin out labels on small scopes
      for(let i=1;i<=5;i++){
        if(!(gap>=24||i===2||i===5))continue;
        const v=String(Math.round(CFG.maxRange*i/5)),x=cx+rad*i/5,y=cy+(tight?4:6)*dpr;
        s.textAlign='center';s.fillText(v,x,y);
        if(i===5){const w=s.measureText(v).width;s.textAlign='left';s.fillText('cm',x+w/2+3*dpr,y)}   // unit sits beside the last number
      }
    }
    const u=small?.65:1;                                                       // rover marker at the origin
    s.beginPath();s.moveTo(cx,cy-9*dpr*u);s.lineTo(cx+6*dpr*u,cy+4*dpr*u);s.lineTo(cx,cy+1.5*dpr*u);s.lineTo(cx-6*dpr*u,cy+4*dpr*u);s.closePath();
    s.fillStyle='rgba('+P+',.95)';s.fill();
  }
  this.fit=function(){
    const p=cv.parentElement,w=p.clientWidth,h=p.clientHeight;
    if(!w||!h){ok=false;return}
    dpr=Math.min(window.devicePixelRatio||1,2);font=getComputedStyle(document.body).fontFamily;
    W=cv.width=Math.round(w*dpr);H=cv.height=Math.round(h*dpr);
    tight=h<170;
    const pT=(small?8:tight?19:26)*dpr,pB=(small?10:tight?17:24)*dpr,pS=(small?8:32)*dpr;
    rad=Math.max(20*dpr,Math.min(W/2-pS,H-pT-pB));
    cx=W/2;cy=(H-(rad+pT+pB))/2+pT+rad;
    build();ok=true;
  };
  function bracket(x,y,s,col,al){
    ctx.save();ctx.translate(x,y);ctx.strokeStyle='rgba('+col+','+al+')';ctx.lineWidth=1.8*dpr;ctx.lineCap='round';
    const l=s*.55;
    for(let i=0;i<4;i++){ctx.rotate(Math.PI/2);ctx.beginPath();ctx.moveTo(s,-s+l);ctx.lineTo(s,-s);ctx.lineTo(s-l,-s);ctx.stroke()}
    ctx.restore();
  }
  this.draw=function(t){
    if(!ok)return;
    ctx.clearRect(0,0,W,H);ctx.drawImage(stat,0,0);
    const lk=R.locked,col=lk?'255,77,94':'77,245,168',a=R.disp;
    // afterglow behind the sweep arm
    const N=small?12:24;
    for(let i=0;i<N;i++){
      const a1=clamp(a-R.dir*i*1.5,0,180),a2=clamp(a-R.dir*(i+1)*1.5,0,180);
      if(a1===a2)continue;
      const c1=ang(a1),c2=ang(a2);
      ctx.beginPath();ctx.moveTo(cx,cy);ctx.arc(cx,cy,rad,Math.min(c1,c2),Math.max(c1,c2));ctx.closePath();
      ctx.fillStyle='rgba('+col+','+(.26*Math.pow(1-i/N,2)).toFixed(3)+')';ctx.fill();
    }
    // echoes, with phosphor persistence
    ctx.globalCompositeOperation='lighter';ctx.lineCap='round';
    for(let i=0;i<R.blips.length;i++){
      const b=R.blips[i],k=Math.max(0,(t-b.t)/LIFE);if(k>=1)continue;
      const c=ramp(b.st,k),al=.3+.7*(1-k),r=b.d/CFG.maxRange*rad,c1=ang(b.a+b.w),c2=ang(b.a-b.w);
      ctx.beginPath();ctx.arc(cx,cy,r,Math.min(c1,c2),Math.max(c1,c2));
      ctx.strokeStyle='rgba('+c+','+(al*.2).toFixed(3)+')';ctx.lineWidth=(small?5:9)*dpr;ctx.stroke();
      ctx.strokeStyle='rgba('+c+','+al.toFixed(3)+')';ctx.lineWidth=(small?1.6:2.6)*dpr;ctx.stroke();
    }
    ctx.globalCompositeOperation='source-over';
    // sweep arm
    const p=pt(a,rad);
    ctx.save();ctx.shadowColor='rgba('+col+',.9)';ctx.shadowBlur=(small?6:14)*dpr;
    ctx.beginPath();ctx.moveTo(cx,cy);ctx.lineTo(p[0],p[1]);
    ctx.strokeStyle=lk?'rgba(255,224,229,.95)':'rgba(222,255,241,.95)';ctx.lineWidth=(small?1.2:1.8)*dpr;ctx.stroke();ctx.restore();
    // target cue and lock-on reticle
    const L=R.last;
    if(L&&(lk||(R.target&&t-L.t<900))){
      const q=pt(L.a,L.d/CFG.maxRange*rad),rc=lk?'255,77,94':'255,176,32';
      if(lk){
        ctx.setLineDash([4*dpr,5*dpr]);ctx.beginPath();ctx.moveTo(cx,cy);ctx.lineTo(q[0],q[1]);
        ctx.strokeStyle='rgba('+rc+',.5)';ctx.lineWidth=dpr;ctx.stroke();ctx.setLineDash([]);
        const k=clamp((t-R.lockAt)/380,0,1),e=1-Math.pow(1-k,3),s=(small?7:11)*dpr+(1-e)*(small?14:30)*dpr;
        bracket(q[0],q[1],s,rc,.35+.65*e);
        ctx.beginPath();ctx.arc(q[0],q[1],1.8*dpr,0,6.2832);ctx.fillStyle='rgba('+rc+',.95)';ctx.fill();
        if(!small){
          const right=q[0]>cx+rad*.45;
          ctx.font='700 '+Math.round(10.5*dpr)+'px '+font;ctx.fillStyle='rgba(255,196,204,.98)';
          ctx.textAlign=right?'right':'left';ctx.textBaseline='bottom';
          ctx.fillText(Math.round(L.d)+' cm',q[0]+(right?-16:16)*dpr,q[1]-12*dpr);
        }
      }else{
        ctx.beginPath();ctx.arc(q[0],q[1],(small?6:10)*dpr*(1+.18*Math.sin(t/140)),0,6.2832);
        ctx.strokeStyle='rgba('+rc+',.85)';ctx.lineWidth=1.5*dpr;ctx.stroke();
      }
    }
  };
}
const radarMain=new Radar(el.radar,false),radarPip=new Radar(el.pip,true);
if(window.ResizeObserver){
  new ResizeObserver(()=>radarMain.fit()).observe($('#vpBody'));
  new ResizeObserver(()=>radarPip.fit()).observe(el.pipWrap);
}else addEventListener('resize',()=>{radarMain.fit();radarPip.fit()});

/* ---------- views: radar / camera / FPV ---------- */
let view='radar',fpv=false,camLoaded=false,camTimer=0,chipKey='';
let camSide=LS.get('trinetra_cam_side','front')||'front'; // 'front' | 'back'
let camUrlFront=LS.get('trinetra_cam_front','')||'';
let camUrlBack=LS.get('trinetra_cam_back','')||'';
// migrate old single-key save
(function(){
  const old=LS.get('trinetra_cam_ip','');
  if(old && !camUrlFront){camUrlFront=old;LS.set('trinetra_cam_front',old)}
})();
function activeCamUrl(){
  const custom=camSide==='back'?camUrlBack:camUrlFront;
  if(custom && custom.trim()) return custom.trim();
  // proxy path — cache-bust so browser reopens socket after ESP32 switches upstream
  return CFG.camProxy + (CFG.camProxy.indexOf('?')>=0?'&':'?') + 'src=' + camSide + '&t=' + Date.now();
}
function syncCamSwitchUI(){
  $$('.cam-switch button').forEach(b=>{
    b.setAttribute('aria-pressed', String(b.dataset.cam===camSide));
  });
}
const hostOf=u=>{try{return new URL(u).host||'FEED LINKED'}catch(e){return u}};
function updateChip(){
  let s,txt;
  if(view==='cam'){
    s=camLoaded?'cam':'wait';
    const side=camSide==='back'?'BACK':'FRONT';
    txt=camLoaded?(side+' · '+hostOf(activeCamUrl())):(side+' · LINKING');
  }
  else if(linkState==='lost'){s='lost';txt='NO SIGNAL'}
  else if(R.locked){s='locked';txt='TARGET LOCKED'}
  else if(R.sweeping){s='scan';txt='SCANNING'}
  else{s='hold';txt='SWEEP HOLD'}
  const k=s+txt;if(k===chipKey)return;chipKey=k;el.chip.dataset.s=s;el.chipTxt.textContent=txt;
}
function loadCam(){
  camLoaded=false;clearTimeout(camTimer);
  const url=activeCamUrl();
  el.camState.classList.remove('hide','fail');
  el.camStateTxt.textContent='ACQUIRING '+(camSide==='back'?'BACK':'FRONT');
  el.camHost.textContent=hostOf(url);
  el.camFrame.src=url;
  syncCamSwitchUI();
  camTimer=setTimeout(()=>{if(camLoaded)return;const im=el.camFrame;if(im&&im.naturalWidth>0){camLoaded=true;el.camState.classList.add('hide');updateChip();return}el.camState.classList.add('fail');el.camStateTxt.textContent='NO FEED YET';el.camHost.textContent='Check camera AP / FRONT·BACK paths in settings';},9000);
  updateChip();
}
function unloadCam(){camLoaded=false;clearTimeout(camTimer);el.camFrame.src='about:blank'}
async function setCamSide(side){
  if(side!=='front'&&side!=='back')return;
  if(side===camSide && camLoaded)return;
  camSide=side;
  LS.set('trinetra_cam_side',side);
  syncCamSwitchUI();
  // Tell ESP32 which upstream to open (only matters when using proxy URLs)
  const usingProxy=!(camSide==='back'?camUrlBack:camUrlFront);
  if(usingProxy && !prefs.sim){
    try{await fetch('/camera?src='+side,{cache:'no-store'})}catch(e){}
  }
  if(view==='cam')loadCam();
  toast((side==='back'?'Back':'Front')+' camera');
  vib(12);
}
el.camFrame.addEventListener('load',()=>{
  if(el.camFrame.getAttribute('src')==='about:blank')return;
  camLoaded=true;clearTimeout(camTimer);el.camState.classList.add('hide');updateChip();
});
function setView(v){
  if(v===view)return;
  view=v;el.vp.dataset.view=v;
  $$('.tab').forEach(t=>t.setAttribute('aria-selected',String(t.dataset.view===v)));
  if(v==='cam')loadCam();else{unloadCam();setFpv(false)}
  radarMain.fit();updateChip();
}
let fsByFpv=false;
const isFs=()=>!!(document.fullscreenElement||document.webkitFullscreenElement);
function setFpv(on){
  if(on&&view!=='cam')return;
  fpv=!!on;document.body.classList.toggle('fpv',fpv);
  if(fpv){
    toast('FPV view. Tap EXIT or press Esc to leave.');
    const de=document.documentElement,f=de.requestFullscreen||de.webkitRequestFullscreen;     // hide the browser bars too, where the phone allows it
    if(f&&!isFs()){try{Promise.resolve(f.call(de)).then(()=>{fsByFpv=true}).catch(()=>{})}catch(e){}}
  }else if(fsByFpv){
    fsByFpv=false;const d=document,x=d.exitFullscreen||d.webkitExitFullscreen;
    if(x&&isFs()){try{x.call(d)}catch(e){}}
  }
  radarMain.fit();radarPip.fit();
}
['fullscreenchange','webkitfullscreenchange'].forEach(n=>document.addEventListener(n,()=>{if(!isFs())fsByFpv=false}));

/* ---------- drive pad ---------- */
const pad=el.pad,knob=el.knob;
let dir='S',padPtr=null;
const DIRNAME={F:'FORWARD',B:'REVERSE',L:'LEFT',R:'RIGHT'};
function setDir(d){
  if(d===dir)return;
  dir=d;pad.dataset.dir=d;
  el.driveState.textContent=d==='S'?'DRAG TO DRIVE':DIRNAME[d];el.driveState.dataset.on=d==='S'?'':'1';
  send('/move?cmd='+d,{quiet:true});                                           // same protocol as before: F / B / L / R / S
  if(d==='S')stopBurst();else vib(8);
}
function stopBurst(){[140,380].forEach(ms=>setTimeout(()=>{if(dir==='S')send('/move?cmd=S',{quiet:true})},ms))}  // repeat STOP: a lost packet must never leave the robot driving
function padMove(x,y){
  const r=pad.getBoundingClientRect(),cx=r.left+r.width/2,cy=r.top+r.height/2,travel=r.width/2-knob.offsetWidth/2;
  let dx=x-cx,dy=y-cy;const dist=Math.hypot(dx,dy);
  if(dist>travel){dx=dx/dist*travel;dy=dy/dist*travel}
  knob.style.transform='translate('+dx+'px,'+dy+'px)';
  let d='S';
  if(dist>travel*.3){const a=Math.atan2(dy,dx)*180/Math.PI;d=a>=-135&&a<-45?'F':a>=-45&&a<45?'R':a>=45&&a<135?'B':'L'}
  setDir(d);
}
function releasePad(){pad.classList.remove('drag');knob.style.transform='';setDir('S')}
pad.addEventListener('pointerdown',e=>{
  if(padPtr!==null)return;
  padPtr=e.pointerId;try{pad.setPointerCapture(e.pointerId)}catch(_){}
  pad.classList.add('drag');padMove(e.clientX,e.clientY);e.preventDefault();
});
pad.addEventListener('pointermove',e=>{if(e.pointerId===padPtr)padMove(e.clientX,e.clientY)});
const padEnd=e=>{if(e.pointerId!==padPtr)return;padPtr=null;releasePad()};
['pointerup','pointercancel','lostpointercapture'].forEach(n=>pad.addEventListener(n,padEnd));

/* keyboard driving (laptop) */
const KMAP={ArrowUp:'F',w:'F',ArrowDown:'B',s:'B',ArrowLeft:'L',a:'L',ArrowRight:'R',d:'R'},held=[];
function kbDrive(){
  if(padPtr!==null)return;
  const last=held[held.length-1],d=last?KMAP[last]:'S';
  const r=pad.getBoundingClientRect(),t=r.width/2-knob.offsetWidth/2,o={F:[0,-1],B:[0,1],L:[-1,0],R:[1,0],S:[0,0]}[d];
  knob.style.transform=d==='S'?'':'translate('+o[0]*t+'px,'+o[1]*t+'px)';
  pad.classList.toggle('drag',d!=='S');setDir(d);
}

/* ---------- fire guard, demo, e-stop ---------- */
const fire={armed:false,on:false,t:0};
function renderFire(){
  const r=el.fireRow;r.dataset.state=fire.on?'on':fire.armed?'armed':'safe';r.dataset.guard=prefs.guard?'1':'0';
  r.querySelector('.k-arm').setAttribute('aria-pressed',String(fire.armed));
  r.querySelector('.k-fire').setAttribute('aria-disabled',String(prefs.guard&&!fire.armed&&!fire.on));
}
function setArmed(v){
  clearTimeout(fire.t);fire.armed=!!v;
  if(v)fire.t=setTimeout(()=>{if(!fire.on){setArmed(false);SFX.safe()}},CFG.armMs);
  renderFire();
}
function estop(){
  held.length=0;pad.classList.remove('drag');knob.style.transform='';padPtr=null;
  dir='S';pad.dataset.dir='S';el.driveState.textContent='DRAG TO DRIVE';el.driveState.dataset.on='';
  send('/move?cmd=S',{quiet:true});stopBurst();
  send('/relay/off',{quiet:true});
  fire.on=false;setArmed(false);
  SFX.stop();vib([50,30,50]);toast('Emergency stop. Motion halted and relay off.','err');
}
let demoT=0;
function setRadarCmd(on){el.kRadarOn.classList.toggle('on',on);el.kRadarOff.classList.toggle('on',!on)}
let radarCmdOn=null;

/* ---------- settings sheet ---------- */
function syncSwitches(){$$('.sw').forEach(b=>b.setAttribute('aria-checked',String(!!prefs[b.dataset.pref])))}
function openSheet(focusCam){
  el.camInputFront.value=camUrlFront||CFG.camProxy;el.camInputBack.value=camUrlBack||CFG.camProxy;syncSwitches();el.sheet.classList.add('open');el.sheet.setAttribute('aria-hidden','false');
  if(focusCam)setTimeout(()=>{el.camInput.focus();el.camInput.select()},320);
}
function closeSheet(){el.sheet.classList.remove('open');el.sheet.setAttribute('aria-hidden','true');if(document.activeElement)document.activeElement.blur()}
function togglePref(k){
  const v=!prefs[k];
  if(k==='sim')setSim(v);else{prefs[k]=v;savePrefs()}
  if(k==='guard')renderFire();
  if(k==='mirror'){radarMain.fit();radarPip.fit()}
  if(k==='sound'&&v){audio();SFX.on()}
  if(k==='haptics'&&v)vib(30);
  syncSwitches();
}
function toggleFs(){
  const d=document,de=d.documentElement;
  if(!(d.fullscreenElement||d.webkitFullscreenElement)){
    const f=de.requestFullscreen||de.webkitRequestFullscreen;
    if(f)Promise.resolve(f.call(de)).then(()=>{try{screen.orientation.lock('landscape').catch(()=>{})}catch(e){}}).catch(()=>{});
  }else{const x=d.exitFullscreen||d.webkitExitFullscreen;if(x)x.call(d)}
}

/* ---------- actions (same endpoints as the original console) ---------- */
const ACT={
  estop:estop,
  horn(){SFX.horn();vib(25);toast('Horn sounded (local)')},
  demo(){
    SFX.demo();send('/demo/start');vib(20);
    const b=el.kDemo;b.classList.remove('run');void b.offsetWidth;b.classList.add('run','on');b.querySelector('span').textContent='Demo running';
    clearTimeout(demoT);demoT=setTimeout(()=>{b.classList.remove('run','on');b.querySelector('span').textContent='3-Min Demo'},CFG.demoMs);
    toast('Demo sequence started. It runs for about 3 minutes.');
  },
  radarOn(){send('/radar/start');radarCmdOn=true;setRadarCmd(true);SFX.on();vib(12);toast('Radar sweep on')},
  radarOff(){send('/radar/stop');radarCmdOn=false;setRadarCmd(false);SFX.off();vib(12);toast('Radar sweep off')},
  lock(){lastTap=tnow();send('/lock');SFX.lock();vib([20,30,20]);toast('Locking target')},
  unlock(){lastTap=tnow();send('/unlock');SFX.unlock();vib(15);toast('Target released')},
  arm(){
    if(!prefs.guard||fire.on)return;
    setArmed(!fire.armed);vib(15);
    if(fire.armed){SFX.arm();toast('Armed. FIRE ON is live for '+Math.round(CFG.armMs/1000)+' seconds.','warn')}else SFX.safe();
  },
  fireOn(){
    if(prefs.guard&&!fire.armed&&!fire.on){toast('Safety is on. Tap ARM first.','warn');vib(25);return}
    send('/relay/on');fire.on=true;clearTimeout(fire.t);renderFire();SFX.fire();vib([40,40,80]);toast('Relay on','warn');
  },
  fireOff(){send('/relay/off');fire.on=false;setArmed(false);SFX.off();vib(20);toast('Relay off')},
  viewRadar(){setView('radar')},
  viewCam(){setView('cam')},
  camReload(){loadCam()},
  camEdit(){openSheet(true)},
  camFront(){setCamSide('front')},
  camBack(){setCamSide('back')},
  fpv(){setFpv(!fpv)},
  settings(){openSheet()},
  closeSheet:closeSheet,
  fs:toggleFs,
  simOn(){setSim(true)},
  camSave(){
    let f=el.camInputFront.value.trim(), b=el.camInputBack.value.trim();
    const norm=u=>{if(!u)return '';if(!/^[a-z][a-z0-9+.-]*:\/\//i.test(u))u='http://'+u;return u};
    f=norm(f); b=norm(b);
    // blank or pure proxy URL → use robot proxy (empty custom)
    const isProxy=u=>!u || u.indexOf('192.168.4.1:81')>=0;
    camUrlFront=isProxy(f)?'':f;
    camUrlBack=isProxy(b)?'':b;
    LS.set('trinetra_cam_front',camUrlFront);
    LS.set('trinetra_cam_back',camUrlBack);
    closeSheet();
    toast('Camera addresses saved');
    if(view==='cam')loadCam();else setView('cam');
  },
  camReset(){
    camUrlFront='';camUrlBack='';
    LS.set('trinetra_cam_front','');LS.set('trinetra_cam_back','');
    el.camInputFront.value=CFG.camProxy;el.camInputBack.value=CFG.camProxy;
    toast('Cameras reset to ESP32 proxy');
    if(view==='cam')loadCam();
  }
};
const run=b=>{const f=ACT[b.dataset.act];if(f)f(b)};

/* ---------- input wiring ---------- */
// Drive-critical keys fire on pointerdown (instant, and safe for multi-touch while the other thumb holds the joystick).
document.addEventListener('pointerdown',e=>{
  const k=e.target.closest('.key,.stop,.ibtn');if(k)k.classList.add('press');
  const b=e.target.closest('[data-fast]');
  if(!b||e.button>0)return;
  e.preventDefault();run(b);
},true);
const unpress=()=>$$('.press').forEach(x=>x.classList.remove('press'));
document.addEventListener('pointerup',unpress,true);document.addEventListener('pointercancel',unpress,true);
document.addEventListener('click',e=>{
  const b=e.target.closest('[data-act]');
  if(b){
    if(b.hasAttribute('data-fast')&&e.detail!==0)return;                       // already handled on pointerdown; this is the keyboard path
    run(b);if(e.detail!==0&&b.blur)b.blur();return;
  }
  const s=e.target.closest('[data-pref]');if(s)togglePref(s.dataset.pref);
});
document.addEventListener('contextmenu',e=>{if(!e.target.closest('input'))e.preventDefault()});

addEventListener('keydown',e=>{
  if(!e.key||(e.target.closest&&e.target.closest('input,textarea')))return;
  if(e.ctrlKey||e.metaKey||e.altKey)return;
  const k=e.key.length===1?e.key.toLowerCase():e.key;
  if(KMAP[k]){e.preventDefault();if(!held.includes(k)){held.push(k);kbDrive()}return}
  if(k===' '){e.preventDefault()}
  if(e.repeat)return;
  if(k===' ')estop();
  else if(k==='h')ACT.horn();
  else if(k==='l')ACT.lock();
  else if(k==='u')ACT.unlock();
  else if(k==='r')(radarCmdOn?ACT.radarOff:ACT.radarOn)();
  else if(k==='c')setView(view==='cam'?'radar':'cam');
  else if(k==='v'){if(view==='cam')setFpv(!fpv)}
  else if(k==='Escape'){if(el.sheet.classList.contains('open'))closeSheet();else if(fpv)setFpv(false)}
});
addEventListener('keyup',e=>{
  if(!e.key)return;
  const k=e.key.length===1?e.key.toLowerCase():e.key;
  if(k===' ')e.preventDefault();
  const i=held.indexOf(k);if(i>-1){held.splice(i,1);kbDrive()}
});

/* ---------- failsafes ---------- */
function failsafe(){if(dir!=='S'||held.length){held.length=0;padPtr=null;releasePad()}}
document.addEventListener('visibilitychange',()=>{if(document.hidden)failsafe()});
addEventListener('pagehide',()=>{if(!prefs.sim){try{fetch('/move?cmd=S',{keepalive:true})}catch(e){}}});

/* ---------- main loop ---------- */
let lastT=0,lastChip=0;
function frame(t){
  requestAnimationFrame(frame);
  if(document.hidden)return;
  const dt=Math.min(.1,(t-lastT)/1000||.016);lastT=t;
  R.disp+=(R.angle-R.disp)*Math.min(1,dt*14);
  R.sweeping=t-R.moveAt<900;
  if(t-lastChip>250){lastChip=t;updateChip()}
  while(R.blips.length&&t-R.blips[0].t>LIFE)R.blips.shift();
  if(fpv)radarPip.draw(t);else if(view==='radar')radarMain.draw(t);
}

/* ---------- start ---------- */
(function init(){
  el.fireRow.style.setProperty('--arm',CFG.armMs/1000+'s');
  el.kDemo.style.setProperty('--demo',CFG.demoMs/1000+'s');
  renderFire();syncSwitches();
  if(window.matchMedia&&matchMedia('(hover:hover) and (pointer:fine)').matches){el.kbdHelp.hidden=false;el.kbdT.hidden=false}
  {const de=document.documentElement;if(!(de.requestFullscreen||de.webkitRequestFullscreen))el.btnFs.hidden=true}
  radarMain.fit();radarPip.fit();
  syncCamSwitchUI();
  requestAnimationFrame(frame);
  if(prefs.sim){setLink('sim',10);setTimeout(()=>toast('Preview mode. Simulated data, nothing is sent to a robot.','sim'),1800)}
  pollOnce();
  const t0=Date.now();
  setInterval(()=>{const s=Math.floor((Date.now()-t0)/1000);el.clock.textContent=[s/3600|0,(s/60|0)%60,s%60].map(n=>String(n).padStart(2,'0')).join(':')},1000);
  // splash: once per browser session, tap to skip
  let seen=false;try{seen=sessionStorage.getItem('trinetra_boot')==='1';sessionStorage.setItem('trinetra_boot','1')}catch(e){}
  if(seen)el.boot.classList.add('done');
  else{el.boot.addEventListener('pointerdown',()=>el.boot.classList.add('done'));setTimeout(()=>el.boot.classList.add('done'),1500)}
})();

})();
</script>
</body>
</html>
)TRINETRAHTML";

#endif