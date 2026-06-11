import * as vscode from 'vscode';
import { getMatchDetail } from '../api/hltv';

export async function openMatchDetail(matchId: string) {
    const panel = vscode.window.createWebviewPanel(
        'hltvMatchDetail',
        `Match ${matchId}`,
        vscode.ViewColumn.One,
        { enableScripts: true }
    );

    const detail = await getMatchDetail(matchId);

    panel.webview.html = render(detail);
}

function render(m: any) {
    return `
    <html>
    <body style="font-family: sans-serif; padding: 20px;">
        <h1>${m.team1.name} vs ${m.team2.name}</h1>
        <h2>Event: ${m.event.name}</h2>
        <h3>Format: BO${m.format}</h3>

        <h2>Maps</h2>
        ${m.maps
            .map(
                (map: any) => `
            <div>
                <h3>${map.name}</h3>
                <p>${map.result.team1} : ${map.result.team2}</p>
            </div>
        `
            )
            .join('')}

        <h2>Players</h2>
        <pre>${JSON.stringify(m.players, null, 2)}</pre>
    </body>
    </html>
    `;
}
