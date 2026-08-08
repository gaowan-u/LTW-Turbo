'use strict';
/* 存储：优先用 Capacitor Preferences（接壳后），不可用时回退浏览器 localStorage */
let prefCache={};
let useCap=false;
async function initStore(){
  const P=window.Capacitor?.Plugins?.Preferences;
  if(P){
    try{
      const {keys}=await P.keys();
      for(const k of keys){
        if(!k.startsWith('ltw.')) continue;
        const {value}=await P.get({key:k});
        if(value!=null) prefCache[k]=JSON.parse(value);
      }
      for(let i=0;i<localStorage.length;i++){
        const k=localStorage.key(i);
        if(k&&k.startsWith('ltw.')&&!(k in prefCache)){
          try{
            const v=localStorage.getItem(k);
            await P.set({key:k,value:v});
            prefCache[k]=JSON.parse(v);
          }catch(e){}
        }
      }
      useCap=true;
    }catch(e){ useCap=false; }
  }
  if(!useCap){
    try{
      for(let i=0;i<localStorage.length;i++){
        const k=localStorage.key(i);
        if(k&&k.startsWith('ltw.')) prefCache[k]=JSON.parse(localStorage.getItem(k));
      }
    }catch(e){}
  }
}
const Store = {
  get(k,d){ const v=prefCache['ltw.'+k]; return v===undefined?d:v; },
  set(k,v){
    prefCache['ltw.'+k]=v;
    const val=JSON.stringify(v);
    if(useCap&&window.Capacitor?.Plugins?.Preferences){
      window.Capacitor.Plugins.Preferences.set({key:'ltw.'+k,value:val}).catch(()=>{});
    }else{
      try{ localStorage.setItem('ltw.'+k,val); }catch(e){}
    }
    if(window.Capacitor?.Plugins?.Filesystem) scheduleSharedConfig();
  },
  async clear(){
    prefCache={};
    const P=useCap&&window.Capacitor?.Plugins?.Preferences;
    if(P){
      try{
        const {keys}=await P.keys();
        for(const k of keys.filter(x=>x.startsWith('ltw.'))) await P.remove({key:k});
      }catch(e){}
    }else{
      try{ Object.keys(localStorage).filter(k=>k.startsWith('ltw.')).forEach(k=>localStorage.removeItem(k)); }catch(e){}
    }
  }
};

// ---- 共享配置桥接（应用写文件，LTW 渲染库自己读）----
const SHARED_CONFIG_PATH='LTW-Turbo/config.json';
let sharedConfigTimer=null;
let sharedConfigPrompted=false;

function scheduleSharedConfig(){
  clearTimeout(sharedConfigTimer);
  sharedConfigTimer=setTimeout(()=>syncSharedConfig(false),300);
}

function buildSharedConfig(){
  const data={};
  SETTINGS.flatMap(g=>g.items).forEach(it=>{
    if(!it.key||!CONFIG_RULES[it.key]) return;
    data[it.key]=Store.get(it.key,it.def);
  });
  data.plan=Store.get('plan',0);
  return data;
}

async function ensureAllFilesAccess(silent){
  const A=window.Capacitor?.Plugins?.AllFilesAccess;
  const FS=window.Capacitor?.Plugins?.Filesystem;
  if(!FS) return true;
  if(A){
    try{
      const {granted}=await A.isGranted();
      if(granted) return true;
    }catch(e){}
  }
  if(!silent&&!sharedConfigPrompted){
    sharedConfigPrompted=true;
    if(A){
      showDialog('需要“所有文件访问”权限',
        'LTW 需要写入共享目录 /sdcard/LTW-Turbo/config.json，渲染库启动时才能读到实验性开关。请授权后返回。',
        '去授权',()=>A.openSettings());
    }else{
      showDialog('需要“所有文件访问”权限',
        '检测不到权限插件，请在系统设置里手动授予本应用“所有文件访问”权限，然后回到本页点“立即写入共享配置”。',
        '知道了',()=>{});
    }
  }
  return false;
}

async function syncSharedConfig(silent){
  const FS=window.Capacitor?.Plugins?.Filesystem;
  if(!FS) return;
  if(!await ensureAllFilesAccess(silent)){
    if(!silent) toast('请先授予“所有文件访问”权限');
    return;
  }
  try{
    await FS.writeFile({
      path:SHARED_CONFIG_PATH,
      data:JSON.stringify(buildSharedConfig()),
      directory:'EXTERNAL_STORAGE',
      recursive:true,
      encoding:'utf8'
    });
  }catch(e){
    if(!silent) toast('共享配置写入失败：'+e.message);
  }
}
const ACCENTS=['#f08c2e','#2e7cf6','#34c77b','#8b5cf6'];
const PLANS=['兼容','性能'];
const VERSION='0.1.0-dev';
let APP_VERSION=VERSION;
async function loadVersion(){
  try{
    const App=window.Capacitor?.Plugins?.App;
    if(App){
      const info=await App.getInfo();
      if(info&&info.version) APP_VERSION=info.version;
    }
  }catch(e){}
}
const LOGO='<svg viewBox="0 0 100 100"><path d="M45 27 L67 45 L86 79 L14 79 Z" fill="var(--accent)" stroke="var(--accent)" stroke-width="10" stroke-linejoin="round"/><path d="M55 12 L68 23 L52 27 Z" fill="var(--accent)" stroke="var(--accent)" stroke-width="8" stroke-linejoin="round"/></svg>';
const ARROW='<svg class="chev" viewBox="0 0 24 24"><path d="M6 9l6 6 6-6"/></svg>';
const CHECK='<svg class="mark" viewBox="0 0 24 24"><path d="M5 12l5 5 9-10"/></svg>';

const SETTINGS=[
 {title:'通用',items:[
  {type:'switch',key:'backDirect',label:'返回键直接退出',def:true}]},
 {title:'外观',items:[
  {type:'select',key:'theme',label:'主题模式',options:['跟随系统','浅色','深色'],def:0},
  {type:'accent',key:'accent',label:'强调色'}]},
 {title:'渲染',items:[
  {type:'select',key:'backend',label:'渲染后端',options:['GLES','Vulkan'],def:0,disable:[1]},
  {type:'switch',key:'dlMerge',label:'显示列表整表合并（实验）',def:true,status:'lab'},
  {type:'action',key:'sharedConfig',label:'立即写入共享配置',status:'lab'}]},
 {title:'缓存',items:[
  {type:'slider',key:'cache',label:'着色器缓存大小',min:64,max:1024,step:64,def:512,unit:' MiB',status:'off'},
  {type:'switch',key:'preheat',label:'缓存预热',def:false,status:'plan'}]},
 {title:'配置',items:[
  {type:'action',key:'export',label:'导出配置'},
  {type:'action',key:'import',label:'导入配置'}]}
];

const CONFIG_APP='LTW-Turbo';
const CONFIG_SCHEMA=1;
const CONFIG_MAX=32768;
const CONFIG_RULES={
  theme:{t:'int',min:0,max:2},
  accent:{t:'color'},
  backend:{t:'int',min:0,max:1},
  cache:{t:'int',min:64,max:1024},
  preheat:{t:'bool'},
  dlMerge:{t:'bool'},
  plan:{t:'int',min:0,max:1},
  backDirect:{t:'bool'}
};
const INFO=[
 {title:'应用',rows:[
  {label:'版本',key:'version'},
  {label:'项目主页',href:'https://github.com/gaowan-u/LTW-Turbo',value:'GitHub 仓库 ↗'}
 ]},
 {title:'关于',rows:[
  {label:'开发者',value:'MathCode（gaowan-u）'},
  {label:'贡献者',value:'MathCode、RED·LIN Perry、NowayLzy'},
  {label:'致谢',value:'原 LTW 团队：artDev、SerpentSpirale、CADIndie'},
  {label:'开源许可证',value:'LGPL-3.0'}
 ]},
 {title:'第三方开源项目',rows:[
  {label:'Capacitor',value:'8.x 系列（npm 声明 ^8.0.0）· Apache-2.0（core、android、cli、app、preferences、browser、filesystem、share）'},
  {label:'AndroidX 组件',value:'AppCompat 1.7.1、Core 1.17.0、Activity 1.11.0、Fragment 1.8.9、WebKit 1.14.0、SplashScreen 1.2.0、CoordinatorLayout 1.3.0（Apache-2.0）'},
  {label:'Mesa / glsl-optimizer',value:'MIT · 本地集成的 GLSL 编译器'},
  {label:'OpenGL / GLES 头文件',value:'Khronos、Mesa · 宽松许可（MIT 类）'},
  {label:'qrcode-generator',value:'MIT · 二维码生成（本地内置）'},
  {label:'jsQR',value:'Apache-2.0 · 二维码识别（本地内置）'},
  {label:'原 LTW 项目',value:'LGPL-3.0 · github.com/MojoLauncher/LTW'},
  {label:'Cordova Android',value:'14.0.1 · Apache-2.0（Capacitor 生成时引入）'},
  {label:'JUnit 4.13.2',value:'EPL · 仅测试'},
  {label:'AndroidX Test',value:'JUnit 1.3.0、Espresso 3.7.0 · Apache-2.0 · 仅测试'}
 ]},
 {title:'合规',rows:[{label:'隐私政策',value:'（占位）'}]}
];

const el=h=>{const t=document.createElement('template');t.innerHTML=h.trim();return t.content.firstChild};
const $=s=>document.querySelector(s);
let current='home'; const stack=[];
let menuRow=null;

function applyAccent(c){ document.documentElement.style.setProperty('--accent',c); document.documentElement.style.setProperty('--accent-soft',c+'24'); }
function applyTheme(i){ document.documentElement.dataset.theme=['auto','light','dark'][i]; }

function closeMenu(){
  const ov=$('#overlay'); if(ov.hidden) return;
  ov.hidden=true;
  const c=menuRow?.querySelector('.chev');
  if(c) c.style.transform='';
  menuRow=null;
  document.querySelector('main').removeEventListener('scroll',closeMenu);
  window.removeEventListener('resize',closeMenu);
}
function showDialog(title,text,okText,onOk){
  const modal=$('#modal');
  $('#dialog-title').textContent=title;
  $('#dialog-text').textContent=text;
  $('#dialog-ok').textContent=okText;
  $('#dialog-ok').onclick=()=>{ modal.hidden=true; Promise.resolve(onOk()).catch(()=>{}); };
  $('#dialog-cancel').onclick=()=>{ modal.hidden=true; };
  modal.onclick=e=>{ if(e.target===modal) modal.hidden=true; };
  modal.hidden=false;
}
function openMenu(anchor,opts,cur,pick,disable=[]){
  const ov=$('#overlay'), menu=$('#menu');
  menuRow=anchor;
  const c=anchor.querySelector('.chev');
  if(c) c.style.transform='rotate(180deg)';
  menu.innerHTML=opts.map((o,i)=>{
    const dis=disable.includes(i);
    return `<button class="${i===cur?'on':''}" data-i="${i}" ${dis?'disabled':''}><span>${o}</span>${i===cur?CHECK:''}${dis?'<span class="tag">暂不支持</span>':''}</button>`;
  }).join('');
  ov.hidden=false;
  const rect=anchor.getBoundingClientRect();
  let top=rect.bottom+6;
  let right=innerWidth-rect.right+18;
  if(right<8) right=8;
  if(top+menu.offsetHeight>innerHeight-8 && rect.top-menu.offsetHeight-6>0) top=rect.top-menu.offsetHeight-6;
  menu.style.top=top+'px';
  menu.style.right=right+'px';
  menu.onclick=e=>{ const b=e.target.closest('button'); if(b&&!b.disabled) pick(+b.dataset.i); closeMenu(); };
  ov.onclick=e=>{ if(e.target===ov) closeMenu(); };
  document.querySelector('main').addEventListener('scroll',closeMenu,{passive:true});
  window.addEventListener('resize',closeMenu);
}

function groupCard(title,items,lab){
  const card=el(`<section class="card" ${lab?'id="lab-card"':''}><div class="card-title">${title}</div></section>`);
  items.forEach(it=>{
    const dis=it.status==='wip'||it.status==='plan'||it.status==='off';
    const tag=it.status==='wip'?'<span class="tag">开发中</span>':it.status==='plan'?'<span class="tag">规划中</span>':'';
    const selIdx=it.options&&Store.get(it.key,it.def)<it.options.length?Store.get(it.key,it.def):it.def;
    let ctrl='';
    if(it.type==='switch'&&it.status!=='off') ctrl=`<label class="switch"><input type="checkbox" ${Store.get(it.key,it.def)?'checked':''} ${dis?'disabled':''}><i></i></label>`;
    if(it.type==='slider'&&it.status!=='off'){ const v=Store.get(it.key,it.def); ctrl=`<div class="slider"><input type="range" min="${it.min}" max="${it.max}" step="${it.step||1}" value="${v}" ${dis?'disabled':''}><span class="value">${v}${it.unit||''}</span></div>`; }
    if(it.type==='select'&&it.status!=='off') ctrl=`<span class="value sel">${it.options[selIdx]}${ARROW}</span>`;
    if(it.type==='accent'&&it.status!=='off') ctrl=`<div class="swatches">${ACCENTS.map(c=>`<button class="swatch ${c===Store.get('accent',ACCENTS[0])?'on':''}" style="background:${c}" data-c="${c}"></button>`).join('')}</div>`;
    if(it.type==='action') ctrl='<span class="value chevron-right">›</span>';
    if(it.status==='off') ctrl='<span class="value off">暂不支持</span>';
    const row=el(`<div class="row${it.status==='off'?' off':''}" data-label="${it.label}"><span class="label">${it.label}${tag}</span>${ctrl}</div>`);
    if(it.type==='switch'&&!dis) row.querySelector('input').onchange=e=>Store.set(it.key,e.target.checked);
    if(it.type==='slider'&&!dis) row.querySelector('input').oninput=e=>{ Store.set(it.key,+e.target.value); row.querySelector('.value').textContent=e.target.value+(it.unit||''); };
    if(it.type==='select'&&!dis) row.onclick=()=>{
      const idx=Store.get(it.key,it.def)<it.options.length?Store.get(it.key,it.def):it.def;
      openMenu(row,it.options,idx,i=>{ Store.set(it.key,i); row.querySelector('.sel').innerHTML=it.options[i]+ARROW; if(it.key==='theme') applyTheme(i); },it.disable||[]);
    };
    if(it.type==='action') row.onclick=()=>{
      if(it.key==='export') openExport();
      else if(it.key==='import') openImport();
      else if(it.key==='sharedConfig') syncSharedConfig(false);
    };
    if(it.type==='accent'&&!dis) row.querySelectorAll('.swatch').forEach(b=>b.onclick=()=>{ applyAccent(b.dataset.c); Store.set('accent',b.dataset.c); row.querySelectorAll('.swatch').forEach(x=>x.classList.toggle('on',x===b)); });
    card.appendChild(row);
  });
  return card;
}

function collectConfig(){
  const data={};
  SETTINGS.flatMap(g=>g.items).forEach(it=>{
    if(!it.key||!CONFIG_RULES[it.key]) return;
    if(it.key==='accent'){ data.accent=Store.get('accent',ACCENTS[0]); return; }
    data[it.key]=Store.get(it.key,it.def);
  });
  data.plan=Store.get('plan',0);
  return {app:CONFIG_APP,schema:CONFIG_SCHEMA,exportedAt:new Date().toISOString(),data};
}
function validateConfigText(text){
  if(typeof text!=='string'||!text.trim()) return {error:'内容为空'};
  if(text.length>CONFIG_MAX) return {error:'配置内容过大'};
  let obj;
  try{ obj=JSON.parse(text); }catch(e){ return {error:'不是合法的 JSON：'+e.message}; }
  if(!obj||typeof obj!=='object'||Array.isArray(obj)) return {error:'配置格式不正确：应为 JSON 对象'};
  if(obj.app!==CONFIG_APP) return {error:'不是 LTW-Turbo 的配置文件'};
  if(obj.schema!==CONFIG_SCHEMA) return {error:'配置版本不受支持'};
  if(!obj.data||typeof obj.data!=='object'||Array.isArray(obj.data)) return {error:'缺少有效的 data 字段'};
  const out={};
  for(const k of Object.keys(obj.data)){
    const rule=CONFIG_RULES[k]; if(!rule) continue;
    const v=obj.data[k]; let ok=false;
    if(rule.t==='bool') ok=typeof v==='boolean';
    else if(rule.t==='int') ok=Number.isInteger(v)&&v>=rule.min&&v<=rule.max;
    else if(rule.t==='color') ok=typeof v==='string'&&/^#[0-9a-fA-F]{6}$/.test(v);
    if(ok) out[k]=v;
  }
  if(!Object.keys(out).length) return {error:'没有可导入的有效配置项'};
  return {data:out};
}
function applyConfig(data){
  Object.entries(data).forEach(([k,v])=>Store.set(k,v));
  applyAccent(Store.get('accent',ACCENTS[0]));
  applyTheme(Store.get('theme',0));
  renderSettings(); renderInfo(); renderHome();
}
function drawQR(text,canvas){
  if(typeof qrcode!=='function') return;
  const qr=qrcode(0,'M');
  qr.addData(text); qr.make();
  const cell=5,quiet=4*cell,n=qr.getModuleCount(),size=n*cell+quiet*2;
  canvas.width=size; canvas.height=size;
  const ctx=canvas.getContext('2d');
  ctx.fillStyle='#fff'; ctx.fillRect(0,0,size,size);
  ctx.fillStyle='#000';
  for(let r=0;r<n;r++)for(let c=0;c<n;c++) if(qr.isDark(r,c)) ctx.fillRect(quiet+c*cell,quiet+r*cell,cell,cell);
}
let toastTimer=null;
function toast(msg){
  const t=$('#toast'); t.textContent=msg; t.hidden=false;
  clearTimeout(toastTimer); toastTimer=setTimeout(()=>t.hidden=true,3000);
}
function openExport(){
  const json=JSON.stringify(collectConfig(),null,2);
  $('#export-json').value=json;
  drawQR(json,$('#export-qr'));
  $('#export-modal').hidden=false;
}
function copyText(text){
  const ok=()=>toast('已复制到剪贴板');
  if(navigator.clipboard&&navigator.clipboard.writeText){
    navigator.clipboard.writeText(text).then(ok).catch(()=>fallbackCopy(text));
  }else fallbackCopy(text);
}
function fallbackCopy(text){
  const ta=$('#export-json'); ta.select(); ta.setSelectionRange(0,text.length);
  try{ document.execCommand('copy'); toast('已复制到剪贴板'); }catch(e){ toast('复制失败，请手动选择文本'); }
}
function downloadText(name,text){
  const blob=new Blob([text],{type:'application/json'});
  const url=URL.createObjectURL(blob);
  const a=document.createElement('a');
  a.href=url; a.download=name; a.click();
  setTimeout(()=>URL.revokeObjectURL(url),2000);
  toast('已开始下载');
}
function openImport(){
  $('#import-json').value=''; $('#import-json').hidden=true;
  $('#import-do').hidden=true; showImportResult('',false);
  $('#import-modal').hidden=false;
}
function showImportResult(msg,err){
  const p=$('#import-result'); p.hidden=!msg; p.textContent=msg; p.classList.toggle('error',!!err);
}
function tryImportText(text){
  const res=validateConfigText(text);
  if(res.error){ showImportResult(res.error,true); return; }
  $('#import-json').value=text; $('#import-json').hidden=false;
  $('#import-do').hidden=false;
  showImportResult('校验通过，点击“导入”应用',false);
}
let scanStream=null;
function stopScan(){ if(scanStream){ scanStream.getTracks().forEach(t=>t.stop()); scanStream=null; } }
function showScanError(msg){ const p=$('#scan-error'); p.textContent=msg; p.hidden=false; }
function scanResult(text){
  stopScan(); $('#scan-modal').hidden=true;
  $('#import-modal').hidden=false;
  tryImportText(text);
}
function openScan(){
  $('#scan-error').hidden=true;
  $('#scan-modal').hidden=false;
  const video=$('#scan-video'), canvas=$('#scan-canvas');
  const ctx=canvas.getContext('2d',{willReadFrequently:true});
  if(!navigator.mediaDevices||!navigator.mediaDevices.getUserMedia){ showScanError('当前环境不支持摄像头，可改用“选择图片识别”'); return; }
  navigator.mediaDevices.getUserMedia({video:{facingMode:'environment'}}).then(stream=>{
    scanStream=stream; video.srcObject=stream; video.play().catch(()=>{});
    (function tick(){
      if($('#scan-modal').hidden) return;
      if(video.readyState>=2){
        canvas.width=video.videoWidth||640; canvas.height=video.videoHeight||480;
        ctx.drawImage(video,0,0,canvas.width,canvas.height);
        const img=ctx.getImageData(0,0,canvas.width,canvas.height);
        const code=jsQR(img.data,img.width,img.height,{inversionAttempts:'attemptBoth'});
        if(code&&code.data){ scanResult(code.data); return; }
      }
      requestAnimationFrame(tick);
    })();
  }).catch(e=>{ showScanError('无法打开摄像头（'+e.name+'），可改用“选择图片识别”'); });
}

function renderSettings(){
  const host=$('#view-settings');
  host.innerHTML='<input id="search" placeholder="搜索设置">';
  SETTINGS.forEach(g=>{ const ok=g.items.filter(i=>!i.status); if(ok.length) host.appendChild(groupCard(g.title,ok,false)); });
  const lab=SETTINGS.flatMap(g=>g.items).filter(i=>i.status);
  if(lab.length) host.appendChild(groupCard('实验室',lab,true));
  $('#search').oninput=e=>{
    const q=e.target.value.trim();
    host.querySelectorAll('.row').forEach(r=>r.style.display=(!q||r.dataset.label.includes(q))?'':'none');
    host.querySelectorAll('.card').forEach(c=>{ const any=[...c.querySelectorAll('.row')].some(r=>r.style.display!=='none'); c.style.display=any?'':'none'; });
  };
}

function renderHome(){
  $('#view-home').innerHTML=`
   <div class="card hero">${LOGO}<h2>LTW-Turbo</h2><p>${APP_VERSION} · 渲染器设置中心</p></div>
   <div class="actions"><button id="act-plan">方案：${PLANS[Math.min(Store.get('plan',0),PLANS.length-1)]}</button><button id="act-search">搜索</button><button id="act-lab">实验室</button></div>
   <section class="card"><div class="card-title">状态概览</div>
    <div class="row"><span class="label">外壳</span><span class="value">Capacitor</span></div>
    <div class="row"><span class="label">主题</span><span class="value">${['跟随系统','浅色','深色'][Store.get('theme',0)]}</span></div>
    <div class="row"><span class="label">强调色</span><span class="value">${Store.get('accent',ACCENTS[0])}</span></div>
   </section>`;
  $('#act-plan').onclick=()=>{ Store.set('plan',(Math.min(Store.get('plan',0),PLANS.length-1)+1)%PLANS.length); renderHome(); };
  $('#act-search').onclick=()=>{ switchView('settings'); setTimeout(()=>$('#search').focus(),50); };
  $('#act-lab').onclick=()=>{ switchView('settings'); setTimeout(()=>$('#lab-card')?.scrollIntoView({behavior:'smooth'}),50); };
}

function renderInfo(){
  const host=$('#view-info'); host.innerHTML='';
  INFO.forEach(g=>{
    const card=el(`<section class="card"><div class="card-title">${g.title}</div></section>`);
    g.rows.forEach(r=>{
      const val=r.key==='version'?APP_VERSION:r.value;
      const row=el(`<div class="row info"><div class="label">${r.label}</div>${r.href?`<a class="value link" href="${r.href}" target="_blank" rel="noopener">${val}</a>`:`<div class="value">${val}</div>`}</div>`);
      if(r.href) row.querySelector('a').onclick=e=>{
        const B=window.Capacitor?.Plugins?.Browser;
        if(B){ e.preventDefault(); B.open({url:r.href}).catch(()=>{}); }
      };
      card.appendChild(row);
    });
    host.appendChild(card);
  });
  host.appendChild(el('<section class="card"><div class="row danger"><span class="label">撤销授权或重置设置</span></div></section>'))
    .onclick=()=>showDialog('撤销授权或重置设置','这会清除全部设置并恢复默认状态，确定要继续吗？','重置',async()=>{ await Store.clear(); location.reload(); });
}

function switchView(name,push=true){
  if(push&&current!==name){ stack.push(current); try{history.pushState({v:name},'')}catch(e){} }
  current=name;
  document.querySelectorAll('.view').forEach(v=>v.classList.toggle('active',v.id==='view-'+name));
  document.querySelectorAll('.tab').forEach(t=>t.classList.toggle('active',t.dataset.view===name));
  $('#page-title').textContent={home:'首页',settings:'设置',info:'信息'}[name];
  if(name==='home') renderHome();
}
function back(){
  if(!$('#modal').hidden){ $('#modal').hidden=true; return; }
  if(!$('#overlay').hidden){ closeMenu(); return; }
  if(!$('#export-modal').hidden){ $('#export-modal').hidden=true; return; }
  if(!$('#scan-modal').hidden){ stopScan(); $('#scan-modal').hidden=true; $('#import-modal').hidden=false; return; }
  if(!$('#import-modal').hidden){ $('#import-modal').hidden=true; return; }
  if(Store.get('backDirect',true)){
    if(window.Capacitor?.Plugins?.App) Capacitor.Plugins.App.exitApp();
    return;
  }
  if(stack.length) switchView(stack.pop(),false);
  else if(window.Capacitor?.Plugins?.App) Capacitor.Plugins.App.exitApp();
}
window.addEventListener('popstate',back);
if(window.Capacitor?.Plugins?.App) Capacitor.Plugins.App.addListener('backButton',back);
document.querySelectorAll('.tab').forEach(t=>t.onclick=()=>switchView(t.dataset.view));

$('#export-copy').onclick=()=>copyText($('#export-json').value);
$('#export-download').onclick=async ()=>{
  const text=$('#export-json').value;
  const FS=window.Capacitor?.Plugins?.Filesystem;
  if(FS){
    try{
      await FS.writeFile({path:'ltw-turbo-config.json',data:text,directory:'DOCUMENTS',encoding:'utf8'});
      toast('已保存到手机“文档”目录');
      return;
    }catch(e){ toast('保存失败：'+e.message); }
  }
  downloadText('ltw-turbo-config.json',text);
};
$('#export-qr-save').onclick=async ()=>{
  const canvas=$('#export-qr');
  const dataUrl=canvas.toDataURL('image/png');
  const FS=window.Capacitor?.Plugins?.Filesystem;
  if(FS){
    try{
      await FS.writeFile({path:'ltw-turbo-config-qr.png',data:dataUrl.split(',')[1],directory:'DOCUMENTS'});
      toast('二维码已保存到手机“文档”目录');
      return;
    }catch(e){ toast('保存失败：'+e.message); }
  }
  const a=document.createElement('a'); a.href=dataUrl; a.download='ltw-turbo-config-qr.png'; a.click();
  toast('已开始下载二维码');
};
$('#export-share').onclick=()=>{
  if(navigator.share) navigator.share({title:'LTW-Turbo 配置',text:$('#export-json').value}).catch(()=>{});
  else toast('当前环境不支持系统分享，可用复制或下载');
};
$('#export-close').onclick=()=>$('#export-modal').hidden=true;
$('#export-modal').onclick=e=>{ if(e.target===$('#export-modal')) $('#export-modal').hidden=true; };

$('#import-scan').onclick=()=>{ $('#import-modal').hidden=true; openScan(); };
$('#import-paste').onclick=()=>{ $('#import-json').hidden=false; $('#import-do').hidden=false; $('#import-json').focus(); };
$('#import-file').onclick=()=>{ $('#import-file-input').value=''; $('#import-file-input').click(); };
$('#import-file-input').onchange=e=>{
  const file=e.target.files[0]; if(!file) return;
  if(file.size>CONFIG_MAX){ showImportResult('文件过大',true); return; }
  const reader=new FileReader();
  reader.onload=()=>tryImportText(reader.result);
  reader.readAsText(file);
};
$('#import-do').onclick=()=>{
  const res=validateConfigText($('#import-json').value);
  if(res.error){ showImportResult(res.error,true); return; }
  applyConfig(res.data);
  showImportResult('导入成功，设置已更新',false);
};
$('#import-close').onclick=()=>$('#import-modal').hidden=true;
$('#import-modal').onclick=e=>{ if(e.target===$('#import-modal')) $('#import-modal').hidden=true; };

$('#scan-file').onclick=()=>{ $('#scan-file-input').value=''; $('#scan-file-input').click(); };
$('#scan-file-input').onchange=e=>{
  const file=e.target.files[0]; if(!file) return;
  const url=URL.createObjectURL(file);
  const img=new Image();
  img.onload=()=>{
    const maxSide=Math.max(img.width,img.height);
    const sizes=[maxSide];
    if(maxSide>1280) sizes.push(1280);
    if(maxSide>640) sizes.push(640);
    let code=null;
    for(const max of sizes){
      const scale=Math.min(1,max/Math.max(img.width,img.height));
      const w=Math.max(1,Math.round(img.width*scale)), h=Math.max(1,Math.round(img.height*scale));
      const canvas=document.createElement('canvas'); canvas.width=w; canvas.height=h;
      const ctx=canvas.getContext('2d',{willReadFrequently:true});
      ctx.drawImage(img,0,0,w,h);
      try{
        code=jsQR(ctx.getImageData(0,0,w,h).data,w,h,{inversionAttempts:'attemptBoth'});
      }catch(err){}
      if(code) break;
    }
    URL.revokeObjectURL(url);
    if(code&&code.data) scanResult(code.data);
    else showScanError('图片里没识别到二维码：请确认图片清晰、二维码完整，最好只截取二维码区域');
  };
  img.onerror=()=>{ URL.revokeObjectURL(url); showScanError('图片读取失败：可能是格式不支持（如 HEIC），请换成 PNG/JPG'); };
  img.src=url;
};
$('#scan-close').onclick=()=>{ stopScan(); $('#scan-modal').hidden=true; $('#import-modal').hidden=false; };
$('#scan-modal').onclick=e=>{ if(e.target===$('#scan-modal')){ stopScan(); $('#scan-modal').hidden=true; $('#import-modal').hidden=false; } };

(async()=>{
  await Promise.all([initStore(),loadVersion()]);
  applyAccent(Store.get('accent',ACCENTS[0]));
  applyTheme(Store.get('theme',0));
  renderSettings(); renderInfo(); switchView('home',false);
  if(window.Capacitor?.Plugins?.Filesystem) ensureAllFilesAccess(false);
})();

if(window.Capacitor?.Plugins?.App){
  window.Capacitor.Plugins.App.addListener('appStateChange',(state)=>{
    if(state.isActive) syncSharedConfig(true);
  });
}
