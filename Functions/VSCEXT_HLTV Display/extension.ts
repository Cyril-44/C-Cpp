import * as vscode from 'vscode';
import { LiveMatchProvider } from './providers/LiveMatchProvider';
import { UpcomingMatchProvider } from './providers/UpcomingMatchProvider';
import { openMatchDetail } from './providers/MatchDetailView';

export function activate(context: vscode.ExtensionContext) {
    const live = new LiveMatchProvider();
    const upcoming = new UpcomingMatchProvider();

    vscode.window.registerTreeDataProvider('hltvLive', live);
    vscode.window.registerTreeDataProvider('hltvUpcoming', upcoming);

    context.subscriptions.push(
        vscode.commands.registerCommand('hltv.openMatchDetail', openMatchDetail)
    );

    setInterval(() => {
        live.refresh();
        upcoming.refresh();
    }, 10000);
}

export function deactivate() {}
