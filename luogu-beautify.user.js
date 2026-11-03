// ==UserScript==
// @name         Luogu Beautify
// @namespace    https://github.com/Cyril-44
// @version      2.2
// @description  Force dark mode to Luogu website
// @author       Hardsoft
// @match        https://www.luogu.com.cn/*
// @match        https://www.luogu.com/*
// @icon         data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw==
// @connect      cn.bing.com
// @connect      cdn.jsdelivr.net
// @grant        GM_xmlhttpRequest
// @require https://cdn.jsdelivr.net/npm/marked@9.1.2/lib/marked.umd.min.js
// @require https://cdn.jsdelivr.net/npm/katex@0.16.8/dist/katex.min.js
// @require https://cdn.jsdelivr.net/npm/marked-katex-extension@5.1.5/lib/index.umd.min.js

// ==/UserScript==

(function() {
    'use strict';

    if (window.location.href.includes("luogu.com.cn/article/") || window.location.href.includes("luogu.com.cn/paste/")) {
        document.querySelectorAll('a[href]').forEach(link => {
            if (link.href.includes('luogu.com/')) {
                link.href = link.href.replace(/luogu\.com\//g, 'luogu.me\/');
            }
        });
    }

    let bgImgUrl = "https://cdn.luogu.com.cn/upload/image_hosting/69iejqyu.png";
    (function () { // 加载 Bing 壁纸
        const today = new Date().toISOString().slice(0, 10); // 格式：YYYY-MM-DD
        const cachedDate = localStorage.getItem('bing_wallpaper_date');
        const cachedUrl = localStorage.getItem('bing_wallpaper_url');
        if (cachedDate === today && cachedUrl) {
            // ✅ 使用缓存
            bgImgUrl = cachedUrl;
            console.log('使用缓存壁纸：', cachedUrl);
        } else {
            // 🔄 请求 Bing 接口
            GM_xmlhttpRequest({
                method: "GET",
                url: "https://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1",
                onload: function(response) {
                    try {
                        const data = JSON.parse(response.responseText);
                        const imgUrl = ("https://cn.bing.com" + data.images[0].url).replaceAll("1920x1080", "UHD");
                        localStorage.setItem('bing_wallpaper_date', today);
                        localStorage.setItem('bing_wallpaper_url', imgUrl);
                        bgImgUrl = imgUrl;
                        console.log('更新壁纸：', imgUrl);
                    } catch (e) {
                        console.error('解析 Bing 壁纸失败', e);
                    }
                }
            });
        }
    })();

    function addCss(css) {
        const style = document.createElement('style');
        style.textContent = css;
        document.head.appendChild(style);
    }
    function editStyle(selector, rules, timeout = 10000, interval = 100) {
        const start = Date.now();

        const timer = setInterval(() => {
            const target = document.querySelector(selector);

            if (target) {
                clearInterval(timer);
                if (!rules) { // 没有提供规则 ==> 删除
                    target.remove();
                } else {
                    for (const rule of rules) {
                        const [prop, val, important] = rule;
                        if (prop && val !== undefined) { // important = true ==> 添加 !important
                            target.style.setProperty(prop, val, important ? "important" : "");
                        } else if (prop !== undefined) { // 没有 val ==> 删除
                            target.style.removeProperty(prop);
                        }
                    }
                }
            } else if (Date.now() - start > timeout) {
                clearInterval(timer);
                console.warn(`元素 ${selector} 未在 ${timeout}ms 内加载完成`);
                return false;
            }
        }, interval);

        return true;
    }
    function onRouteChange(callback) {
        let lastUrl = location.href;
        // 劫持 pushState 和 replaceState
        const _pushState = history.pushState;
        const _replaceState = history.replaceState;
        history.pushState = function () { _pushState.apply(this, arguments); trigger(); };
        history.replaceState = function () { _replaceState.apply(this, arguments); trigger(); };

        window.addEventListener('popstate', trigger);
        function trigger() {
            const currentUrl = location.href;
            if (currentUrl !== lastUrl) {
                lastUrl = currentUrl;
                callback(currentUrl);
            }
        }
    }
    function waitFor(selector, callback) {
        const el = document.querySelector(selector);
        if (el) return callback(el);

        const observer = new MutationObserver(() => {
            const el = document.querySelector(selector);
            if (el) {
                observer.disconnect();
                callback(el);
            }
        });

        observer.observe(document.body, { childList: true, subtree: true });
    }
    function addSensitivePageOverlay() {
        const overlay = document.createElement('div');
        Object.assign(overlay.style, {
            position: 'fixed',
            inset: '0',
            width: '100vw',
            height: '100vh',
            zIndex: '2147483647',
            backdropFilter: 'blur(40px)',
            WebkitBackdropFilter: 'blur(40px)',
            pointerEvents: 'auto',
        });
        document.documentElement.appendChild(overlay);
        window.setTimeout(() => overlay.remove(), 15000);
    }

    // 设置主页 style
    editStyle('main.lfe-body.mobile-body', [['background', 'transparent'], ['filter']]);
    editStyle('#app > .lfe-body', [['background-color']]);
    editStyle('#app > div.main-container > div.wrapper.wrapped.lfe-body.header-layout.tiny', [['background', 'transparent', true], ['filter']]);
    editStyle('.header-layout.tiny', [['z-index', '30']]);

    function addIntro() { // 添加个人主页的 Introduction
        function extractIntroduction() {
            const scriptElement = document.getElementById('lentille-context');
            if (!scriptElement) { console.error('Script tag #lentille-context not found.'); return null; }
            return JSON.parse(scriptElement.textContent).data.user.introduction;
        }

        function renderAndInsert(introText) {
            if (!window.marked || !window.katex || !window.markedKatex) {
                console.error('Required libraries not loaded.');
                return;
            }

            // --- 1. KaTeX 扩展 ---
            const katexExtension = window.markedKatex({
                throwOnError: false,
                errorColor: '#f00',
                inlineDelimiters: [{ left: '$', right: '$', display: false }],
                blockDelimiters: [{ left: '$$', right: '$$', display: true }],
            });
            window.marked.use(katexExtension);

            // --- 3. 解析 Markdown ---
            const finalHTML = window.marked.parse(introText);

            // --- 4. 插入 DOM ---
            waitFor('#app > .main-container > main > .main > .sidebar-container > .main', (targetContainer) => {
                const cardDiv = document.createElement('div');
                cardDiv.className = 'l-card';
                cardDiv.style.cssText = `
                    margin-bottom: 20px;
                    border: 1px solid #e0e0e0;
                    border-radius: 8px;
                    overflow: hidden;
                `;

                const headerDiv = document.createElement('div');
                headerDiv.className = 'lfe-marked-wrap introduction';
                headerDiv.style.cssText = `
                    padding: 12px 16px;
                    border-bottom: 1px solid #e0e0e0;
                    font-weight: bold;
                `;
                headerDiv.textContent = '个人介绍';

                const contentDiv = document.createElement('div');
                contentDiv.className = 'lfe-marked-wrap introduction';
                contentDiv.style.cssText = `
                    padding: 16px;
                    line-height: 1.5;
                `;
                contentDiv.innerHTML = finalHTML;
                if (unsafeWindow.Prism) { unsafeWindow.Prism.highlightAllUnder(contentDiv); } // 代码高亮

                // --- 5. 限制图片最大宽度 ---
                contentDiv.querySelectorAll('img').forEach(img => {
                    img.style.maxWidth = '100%';
                    img.style.height = 'auto';
                    img.style.display = 'block';
                    img.style.margin = '10px 0';
                });

                // --- 6. 给代码块加上 hljs 样式类 ---
                contentDiv.querySelectorAll('pre code').forEach(block => {
                    block.classList.add('hljs');
                });

                cardDiv.appendChild(headerDiv);
                cardDiv.appendChild(contentDiv);
                targetContainer.insertBefore(cardDiv, targetContainer.firstChild);
            });
        }


        const introTxt = extractIntroduction();
        console.log(introTxt);
        renderAndInsert(introTxt);
    };

    if (window.matchMedia('(prefers-color-scheme: dark)').matches) {
        editStyle('.header-layout.tiny', [['background', 'linear-gradient(90deg, rgba(35, 37, 38, 0.5), rgba(65, 67, 69, 0.5))', true]]);
        editStyle('.l-card.card-content.error-background', [['background', '#20202050 url(https://cdn.luogu.com.cn/errpage/gg1.svg) no-repeat center center', true]]);
        addCss(`
:root {
  --lgb-bg-trans: #00000058;
  --lgb-bg-focus: #000000A0;
  --lgb-bg-card:  #202020A0;
}

a[href^="http"] { color: #56cbe3; }
a:hover, .title:hover { color: #4bc4f2 !important; }
ul>li.selected>span { color: aqua !important; }
.token.punctuation { color: #b8b5ce; }
.center[data-v-1a591deb] { background: #1e2a38 !important; }
.main-wrapper { background: var(--lfe-color--grey-5) !important; }
button.solid:hover, button:hover, li:hover, .item:hover, ::selection, .cs-dialog-timeline-item:hover
  { background: rgba(52, 192, 219, .3) !important; }
.status>.status-link>.status-name
  { color: black !important; }
strong, input, textarea, .refined-input, .background
  { color: white; }
.am-form input[type=number], .am-form input[type=search], .am-form input[type=text], .am-form input[type=password], .am-form input[type=datetime], .am-form input[type=datetime-local], .am-form input[type=date], .am-form input[type=month], .am-form input[type=time], .am-form input[type=week], .am-form input[type=email], .am-form input[type=url], .am-form input[type=tel], .am-form input[type=color], .am-form select, .am-form textarea, .am-form-field { color: white; }
button.solid, button, .swal2-popup, .dropdown, .lform-size-middle,
    button.solid, button, .swal2-popup, .dropdown, .lform-size-middle
  { color: #CCC; }
.count, .result-count, .time, .action>*, .wrap>.name, .info-content, .stat>.field, .detail,
    button>span, .cs-dialog-button, .update-info, .label, .apps a, .radio-group > *, .button-2line>.text,
    .cs-header, .cs-footer, .cm-gutters, .cm-activeLineGutter, .CodeMirror-gutters, .mp-editor-toolbar,
    .header-link, .title, .number, .cm-hr, .row.title>a, .light-black
  { color: #CCC !important; }
code, /* span[data-v-386007e1], */.row>div>span, input
  { color: #cdcdcd !important; }
.lg-article, .item, .content
  { color: #d2d2d2; }
.l-card, .card, .reply-item>.meta, .popup-wrap > .popup
  { color: #dcdcdc; }
.theme-bg, .top-bar, .header-layout.normal > .background, .training-title, .feed-card-meta, .toggle-tag:not(.selected)
  { color: #dcdcdc !important; }
.lfe-body, .swal2-title, .lfe-caption, .caption, .main-container, main, .article-content,
    h1, h2, h3, h4, h5, h6, li, .lfe-h1, .lfe-h2, .lfe-h3, .lfe-h4, .lfe-h5, .lfe-h6
  { color: #dedede; }
svg, a, p
  { color: #e0e0e0; }
.user-nav>a, .header-layout.normal>.header>.user-nav, .header-layout.narrow>.header>.user-nav,
    .content,
    .date-picker-dropdown .calendar-days .day:not(:hover), .date-picker-dropdown .calendar-years .year:not(:hover),
    .time-picker-dropdown .time-column .column-list .time-item:not(:hover), .feed-card-content
  { color: #e0e0e0 !important; }
    .casket, .mp-preview-area,  .mp-preview-content,
    button.solid:hover, button:hover, li:hover, .item:hover, ::selection, .cs-dialog-timeline-item:hover,
    .date-picker-dropdown .calendar-header .current-month:not(:hover)
  { color: #efefef !important; }

.cm-content, .CodeMirror { background: #686868A0 !important; }
.panel-layout>.panel-divider.with-icon { background: #00888880 !important; }
.panel-layout>.panel-divider.dragging { background-color: var(--lfe-color--primary) !important; }
.expand { background: linear-gradient(rgba(35, 37, 38, 0), rgba(65, 67, 69, 0.6)) !important; }
body { background: #202020; }
.page-bar > div > button:not(.selected) { background: var(--lgb-bg-focus) !important; }

.theme-bg, .top-bar, .header-layout.normal > .background { background: linear-gradient(90deg, rgba(15, 17, 18, 0.75), rgba(35, 37, 39, 0.75)) !important; }
.lside.bar, .rside, #app > .lfe-body, .lside.drawer { background: linear-gradient(3deg, rgba(35, 37, 39, 0.75), rgba(15, 17, 18, 0.75)) !important; }
code { border: 0.5px solid white; }
p > img { filter: invert(1); }

`);
    } else {
        editStyle('.header-layout.tiny', [['background', 'linear-gradient(90deg, rgba(217, 219, 220, 0.5), rgba(224, 226, 227, 0.5))', true]]);
        editStyle('.header-layout[data-v-f265fec6] > h1:first-child', [['color', '#222', true]]);
        addCss(`
:root {
  --lgb-bg-trans: #FFFFFF58;
  --lgb-bg-focus: #DFDFDFD0;
  --lgb-bg-card:  #FEFEFEA0;
}

.theme-bg, .top-bar, .header-layout.normal > .background { background: linear-gradient(90deg, rgba(217, 219, 220, 0.75), rgba(224, 226, 227, 0.75)) !important; }
.lside.bar, .rside, #app > .lfe-body, .lside.drawer { background: linear-gradient(3deg, rgba(216, 219, 221, 0.75), rgba(224, 226, 228, 0.75)) !important; }
/* #app > .lfe-body, .footer, .footer a, .header-layout * { color: #333 !important; } */
.footer a:hover { color: rgba(0, 0, 0, 1) !important; }
`);
    }
    addCss(`
.cs-full-screen {
    height: calc(100% - 56px) !important;
    width: calc(100% - 65px) !important;
    backdrop-filter: blur(5px);
    transform: translateY(56px) translateX(65px);
}
.category ul.luogu:not(.selected),
    .CodeMirror pre, .frame,
    .main-container, main, .ide-toolbar,
    code, .row>div>span,
    .block > .item, .dropdown-operations>ul li.active,
    .setting-dropdown, .user-header-bottom,
    .theme-page
  { background: transparent !important; }
.am-panel, .am-form textarea
  { background: var(--lgb-bg-trans); }
.header-layout.normal>.header>.user-nav, .header-layout.narrow>.header>.user-nav,
    .message-block > .message, pre, .inner-card, .combo-wrapper>.text, .drop, .image-block,
    input, textarea, .l-card > .author, .cs-dialog, .problem-order, .dropdown-operations,
    .filter-tags .tag-button:not(:hover), .toggle-tag:not(.selected)
  { background: var(--lgb-bg-trans) !important; }
.am-form textarea:focus, .lfe-code
  { background: var(--lgb-bg-focus); }
.refined-input, .background, .main-container > footer,
    .float-bottom, .bottom-wrap.float,
    .solution-article .operations.float,
    .btn-float-open,
    .category-card.active,
    .step-block .item .number
  { background: var(--lgb-bg-focus) !important; }
.lg-article, .am-comment-bd, .am-comment-hd, .item
  { background: var(--lgb-bg-card); }
.casket, .cs-header, .cs-footer, .cm-gutters, .cm-activeLineGutter, .cm-tooltip,
    .mp-editor-toolbar, .mp-preview-area, .CodeMirror-gutters,
    .l-card, .card, .reply-item>.meta, .popup-wrap > .popup, .error-background,
    .swal2-popup, .dropdown, .ide-container,
    .am-dropdown-content, .am-modal-dialog,
    .category-card, .training-card, .upload-card,
    .date-picker-dropdown, .date-picker-wrapper>.text, .time-picker-dropdown, .time-picker-wrapper>.text,
    .feed-card
  { background-color: var(--lgb-bg-card) !important; }

.step-block .item.active .number[data-v-d9ba08d0] { background-color: var(--lfe-color--primary) !important; }

.l-card.burger.solution-list
  { backdrop-filter: none; }
/* .theme-bg, .top-bar, .header-layout.normal > .background,
    .lside.bar, .rside, #app > .lfe-body, footer,
    .l-button, button.solid, button, .swal2-popup, .lform-size-middle,
    button.solid, button, .swal2-popup, .dropdown, .lform-size-middle,
    button.solid:hover, button:hover, li:hover, .item:hover, ::selection, .cs-dialog-timeline-item:hover,
    .lg-article, .am-comment-bd, .am-comment-hd, .item,
    .l-card, .card, .reply-item>.meta, .popup-wrap > .popup,
    .lside.drawer.show
  { backdrop-filter: blur(40px); } */
.swal2-container.swal2-backdrop-show, .swal2-container.swal2-shown, .background,
    .cs-dialog-container, .cs-dialog, .ide-container, .cm-tooltip, .user-header-bottom,
    .solution-article .operations.float,
    .float-bottom, .bottom-wrap.float,
    .lside.bar, .rside, .top-bar, .dropdown,
    .popup-wrap > .popup,
    .am-dropdown-content, .am-modal-dialog,
    .date-picker-dropdown, .date-picker-wrapper>.text, .time-picker-dropdown, .time-picker-wrapper>.text
  { backdrop-filter: blur(5px); }

/* .header-layout.normal { z-index: 30; }
.header-layout.normal>.header>.user-nav, .header-layout.narrow>.header>.user-nav { z-index: 31; }
.tooltip { position: static !important; } */

.theme-page.theme-frosted .l-card::before {
  background-color: unset;
  backdrop-filter: unset;
}
.theme-page::before {
  background-color: unset;
  background-size: unset;
  background-image: unset;
  mask-image: unset;
}

input, textarea { border: 1px solid #3498db; }
.cs-main { border: 1px solid #3498db !important; }
.center[data-v-1a591deb] { border: 2px solid #3498db !important; }
pre > code { border: unset; }
.category ul.luogu[data-v-62df9252] { background-color: unset !important; }
body { background: none !important; }
.lg-article, .item, .article-content,
    .am-comment-primary .am-comment-main
  { border-radius: 16px; }
.list > .item, .block > .item
  { border-radius: 0px; }
.am-comment-hd, .l-card > .author {
  border-top-left-radius: 16px;
  border-top-right-radius: 16px;
}
.am-comment-bd {
  border-bottom-left-radius: 16px;
  border-bottom-right-radius: 16px;
}
.article-content { max-width: 1300px !important; }
.ide-container { height: 100vh !important; }
`);
    const div = document.createElement('div');
    Object.assign(div.style, {
        content: '',
        position: 'fixed',
        left: '0',
        right: '0',
        display: 'block',
        width: '100%',
        height:  '100%',
        zIndex: '-1',
        background: `url(${bgImgUrl})`,
        backgroundSize: 'cover',          // 覆盖整个区域
        scale: '1.1',
        filter: 'blur(20px) brightness(0.7)',
    });
    document.documentElement.insertBefore(div, document.documentElement.firstChild);
    const dynamicUpdate = (url) => {
        if (/^https:\/\/www\.luogu\.com\.cn\/user\/\d+$/.test(url)) {
            waitFor('canvas#rating-chart', (xyz) => { addIntro(); });
            console.log("Intro should have been added.");
        }
        else if (/^https:\/\/www\.luogu\.com\.cn\/article\/.+/.test(url) || /^https:\/\/www\.luogu\.com\/article\/.+/.test(url)) {
            waitFor('div.actions.left-mode', (elem) => {
                // elem.classList.remove('left-mode');
                elem.remove();
                editStyle('div[style*="rgb(255, 255, 255)"]', [['background-color']]);
                if (document.body.firstChild.classList == undefined) {
                    editStyle('div[data-v-fc349d1c][data-v-5809c34c-s][style*="background-color: rgb(255, 255, 255)"]', [['background-color']]);
                    console.log("Hit " + url + " !");
                    console.log("Inserting element...");
                    const div = document.createElement('div');
                    Object.assign(div.style, {
                        content: '',
                        position: 'fixed',
                        left: '0',
                        right: '0',
                        display: 'block',
                        width: '100%',
                        height:  '100%',
                        zIndex: '-1',
                        background: 'var(--lgb-bg-focus)',
                        // backdropFilter: 'blur(40px)',
                    });
                    document.body.insertBefore(div, document.body.firstChild);
                    console.log("Inserted!");
                };
            });
            (function() { // 添加专栏复制按钮
                'use strict';
                waitFor(".actions", (actions) => {
                    const lastBtn = actions.querySelector(".button-2line:last-child");
                    const newBtn = lastBtn.cloneNode(true);
                    newBtn.querySelector(".text").textContent = "复制源码";
                    const svg = newBtn.querySelector("svg");
                    svg.setAttribute("viewBox", "0 0 512 512");
                    svg.innerHTML = `
                        <path fill="currentColor"
                            d="M71 392l144-144L71 104l33-33 177 177L104 425zm203 0l144-144-144-144 33-33 177 177-177 177z">
                        </path>
                    `;
                    newBtn.onclick = () => {
                        try {
                            const raw = JSON.parse(
                                document.getElementById("lentille-context").innerText
                            ).data.article.content;

                            console.log(raw);
                            navigator.clipboard.writeText(raw);
                            alert("已复制专栏源码到剪贴板，并输出到控制台！");
                        } catch (e) {
                            alert("复制失败：" + e);
                        }
                    };
                    actions.appendChild(newBtn);
                });
            })();
        } else if (url.includes("/problem/solution/")) {
            editStyle('.columba-content-wrap.main-content > .sidebar-container.reverse > .main', [['flex']]);
            waitFor('.columba-content-wrap.main-content > .sidebar-container.reverse > .side', (elem) => { elem.remove(); });
        } else if (url.includes("/user/setting/preference")) {
            console.log("[LockTrigger] 命中敏感页面");
            addSensitivePageOverlay();
            location.href = "cyrillock://trigger?from=luogu";
        } else {
            if (document.body.firstChild.classList !== undefined) {
                console.log("Unhit " + url + " !");
                document.body.removeChild(document.body.firstChild);
            }
        }
    };
    dynamicUpdate(window.location.href);
    onRouteChange(dynamicUpdate);
})();