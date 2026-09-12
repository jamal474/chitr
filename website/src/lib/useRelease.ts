import { useEffect, useState } from 'react';
import { FALLBACK_VERSION, LATEST_URL, REPO } from '@/data/site';

export type PlatformId = 'macos' | 'windows' | 'linux';

export interface PlatformDownload {
  id: PlatformId;
  label: string;
  note: string;
  url: string;
  size: string | null;
}

export interface ReleaseState {
  version: string;
  publishedAt: string | null;
  downloads: Record<PlatformId, PlatformDownload>;
  resolved: boolean;
}

interface GithubAsset {
  name: string;
  browser_download_url: string;
  size: number;
}

interface GithubRelease {
  tag_name: string;
  published_at: string;
  assets: GithubAsset[];
}

const PLATFORMS: { id: PlatformId; label: string; note: string }[] = [
  { id: 'macos', label: 'macOS', note: 'Apple silicon · .app bundle' },
  { id: 'windows', label: 'Windows', note: 'x86_64 · portable zip' },
  { id: 'linux', label: 'Linux', note: 'x86_64 · needs wxWidgets 3.2' },
];

function emptyDownloads(): Record<PlatformId, PlatformDownload> {
  return PLATFORMS.reduce((acc, p) => {
    acc[p.id] = { ...p, url: LATEST_URL, size: null };
    return acc;
  }, {} as Record<PlatformId, PlatformDownload>);
}

function formatSize(bytes: number) {
  return `${(bytes / 1024 / 1024).toFixed(1)} MB`;
}

/**
 * Reads the newest GitHub release so version, date and per-platform asset
 * links stay current. Falls back to the releases page if the call fails.
 */
export function useRelease(): ReleaseState {
  const [state, setState] = useState<ReleaseState>({
    version: FALLBACK_VERSION,
    publishedAt: null,
    downloads: emptyDownloads(),
    resolved: false,
  });

  useEffect(() => {
    let cancelled = false;

    fetch(`https://api.github.com/repos/${REPO}/releases/latest`, {
      headers: { Accept: 'application/vnd.github+json' },
    })
      .then((res) => (res.ok ? (res.json() as Promise<GithubRelease>) : Promise.reject(res.status)))
      .then((release) => {
        if (cancelled) return;
        const downloads = emptyDownloads();
        PLATFORMS.forEach((p) => {
          const match = release.assets.find((a) => a.name.toLowerCase().includes(p.id));
          if (match) {
            downloads[p.id] = {
              ...p,
              url: match.browser_download_url,
              size: formatSize(match.size),
            };
          }
        });
        setState({
          version: release.tag_name.replace(/^v/, ''),
          publishedAt: release.published_at,
          downloads,
          resolved: true,
        });
      })
      .catch(() => {
        if (!cancelled) setState((s) => ({ ...s, resolved: true }));
      });

    return () => {
      cancelled = true;
    };
  }, []);

  return state;
}

/** Best guess at the visitor's platform, used to pick the primary button. */
export function detectPlatform(): PlatformId | null {
  if (typeof navigator === 'undefined') return null;
  const ua = `${navigator.userAgent} ${navigator.platform ?? ''}`.toLowerCase();
  if (ua.includes('mac')) return 'macos';
  if (ua.includes('win')) return 'windows';
  if (ua.includes('linux') || ua.includes('x11')) return 'linux';
  return null;
}
