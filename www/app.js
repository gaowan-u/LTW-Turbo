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
 {title:'外观',items:[
  {type:'select',key:'theme',label:'主题模式',options:['跟随系统','浅色','深色'],def:0},
  {type:'accent',key:'accent',label:'强调色'}]},
 {title:'渲染',items:[
  {type:'select',key:'backend',label:'渲染后端',options:['GLES','Vulkan'],def:0,disable:[1]}]},
 {title:'缓存',items:[
  {type:'slider',key:'cache',label:'着色器缓存大小',min:64,max:1024,step:64,def:512,unit:' MiB',status:'off'},
  {type:'switch',key:'preheat',label:'缓存预热',def:false,status:'plan'}]}
];
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
  {label:'Capacitor',value:'8.x 系列（npm 声明 ^8.0.0）· Apache-2.0（core、android、cli、app、preferences、browser）'},
  {label:'AndroidX 组件',value:'AppCompat 1.7.1、Core 1.17.0、Activity 1.11.0、Fragment 1.8.9、WebKit 1.14.0、SplashScreen 1.2.0、CoordinatorLayout 1.3.0（Apache-2.0）'},
  {label:'Mesa / glsl-optimizer',value:'MIT · 本地集成的 GLSL 编译器'},
  {label:'OpenGL / GLES 头文件',value:'Khronos、Mesa · 宽松许可（MIT 类）'},
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
    if(it.status==='off') ctrl='<span class="value off">暂不支持</span>';
    const row=el(`<div class="row${it.status==='off'?' off':''}" data-label="${it.label}"><span class="label">${it.label}${tag}</span>${ctrl}</div>`);
    if(it.type==='switch'&&!dis) row.querySelector('input').onchange=e=>Store.set(it.key,e.target.checked);
    if(it.type==='slider'&&!dis) row.querySelector('input').oninput=e=>{ Store.set(it.key,+e.target.value); row.querySelector('.value').textContent=e.target.value+(it.unit||''); };
    if(it.type==='select'&&!dis) row.onclick=()=>{
      const idx=Store.get(it.key,it.def)<it.options.length?Store.get(it.key,it.def):it.def;
      openMenu(row,it.options,idx,i=>{ Store.set(it.key,i); row.querySelector('.sel').innerHTML=it.options[i]+ARROW; if(it.key==='theme') applyTheme(i); },it.disable||[]);
    };
    if(it.type==='accent'&&!dis) row.querySelectorAll('.swatch').forEach(b=>b.onclick=()=>{ applyAccent(b.dataset.c); Store.set('accent',b.dataset.c); row.querySelectorAll('.swatch').forEach(x=>x.classList.toggle('on',x===b)); });
    card.appendChild(row);
  });
  return card;
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
  if(stack.length) switchView(stack.pop(),false);
  else if(window.Capacitor?.Plugins?.App) Capacitor.Plugins.App.exitApp();
}
window.addEventListener('popstate',back);
if(window.Capacitor?.Plugins?.App) Capacitor.Plugins.App.addListener('backButton',back);
document.querySelectorAll('.tab').forEach(t=>t.onclick=()=>switchView(t.dataset.view));

(async()=>{
  await Promise.all([initStore(),loadVersion()]);
  applyAccent(Store.get('accent',ACCENTS[0]));
  applyTheme(Store.get('theme',0));
  renderSettings(); renderInfo(); switchView('home',false);
})();
