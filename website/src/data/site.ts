export const REPO = 'jamal474/chitr';
export const REPO_URL = `https://github.com/${REPO}`;
export const RELEASES_URL = `${REPO_URL}/releases`;
export const LATEST_URL = `${RELEASES_URL}/latest`;
export const ISSUES_URL = `${REPO_URL}/issues`;

/** Shipped version, used until the live release lookup resolves. */
export const FALLBACK_VERSION = '1.1.0';

export const YOUTUBE_ID = '8d4SarF3NFY';
export const YOUTUBE_URL = `https://youtu.be/${YOUTUBE_ID}`;

export const AUTHOR = {
  name: 'Md Shabbir Jamal',
  site: 'https://shabbirjamal.com',
};

/** Screenshots are served from the short domain's asset route. */
const SCREENSHOT_BASE =
  import.meta.env.VITE_SCREENSHOT_BASE || 'https://sabo.sh/storage/project/chitr';

export function shot(name: string) {
  return `${SCREENSHOT_BASE}/${name}.png`;
}
