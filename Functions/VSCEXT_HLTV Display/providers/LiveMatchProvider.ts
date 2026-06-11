import * as vscode from 'vscode';
import { getLiveMatches } from '../api/hltv';

export class LiveMatchProvider implements vscode.TreeDataProvider<MatchItem> {
    private _onDidChangeTreeData = new vscode.EventEmitter<void>();
    readonly onDidChangeTreeData = this._onDidChangeTreeData.event;

    refresh() {
        this._onDidChangeTreeData.fire();
    }

    async getChildren() {
        const matches = await getLiveMatches();
        return matches.map((m: any) =>
            new MatchItem(
                `${m.team1.name} ${m.result.team1} : ${m.result.team2} ${m.team2.name}`,
                m.id
            )
        );
    }

    getTreeItem(item: MatchItem) {
        return item;
    }
}

export class MatchItem extends vscode.TreeItem {
    constructor(label: string, public matchId: string) {
        super(label, vscode.TreeItemCollapsibleState.None);
        this.command = {
            command: 'hltv.openMatchDetail',
            title: 'Open Match Detail',
            arguments: [matchId]
        };
    }
}
