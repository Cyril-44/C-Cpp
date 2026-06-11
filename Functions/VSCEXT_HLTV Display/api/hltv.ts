import axios from 'axios';

export async function getLiveMatches() {
    const res = await axios.get('https://hltv-api.vercel.app/api/matches/live');
    return res.data || [];
}

export async function getUpcomingMatches() {
    const res = await axios.get('https://hltv-api.vercel.app/api/matches');
    return res.data || [];
}

export async function getMatchDetail(id: string) {
    const res = await axios.get(`https://hltv-api.vercel.app/api/match/${id}`);
    return res.data || {};
}
