# Chitr landing page

The marketing page for Chitr, served at **https://sabo.sh/chitr/**. It describes
the app, embeds the demo video, and links each platform's download straight to
the newest GitHub release.

React 18 + TypeScript, Vite, Tailwind CSS and Framer Motion. Deployed on
Netlify; the C++ application in the rest of this repository is not built here.

## Local development

```shell
cd website
npm install
npm run dev
```

The dev server runs at http://localhost:5174/chitr/. The sub-path matters: a
bare `/` will 404. `/chitr` without the trailing slash is redirected by a small
dev-only plugin in `vite.config.ts`, matching what Netlify and the edge router
do in production.

| Command | What it does |
| --- | --- |
| `npm run dev` | Vite dev server with hot reload |
| `npm run build` | Type-check, then build into `build/chitr/` |
| `npm run serve` | Serve the production build locally |
| `npm run typecheck` | Type-check only |

## The `/chitr/` base path

The page is served from a sub-path of a short domain that sits behind a
Cloudflare Worker, and that worker forwards paths verbatim rather than
rewriting them. So the build has to produce files whose on-disk paths already
match their public URLs:

- `vite.config.ts` sets `base` to `/chitr/` and builds into `build/chitr/`.
- Netlify publishes `build/`, so `build/chitr/index.html` is served at
  `/chitr/`.
- `netlify.toml` in the repository root redirects `/chitr` to `/chitr/`, adds an
  SPA fallback scoped to `/chitr/*`, and sends anything left at the root into
  `/chitr/`.

`BASE_PATH` overrides the prefix at build time; it must start and end with a
slash.

Reference files by public path with the `asset()` helper in
[`src/lib/utils.ts`](src/lib/utils.ts) rather than hard-coding `/chitr/`, so a
changed base path stays a one-line edit.

## Deployment

Netlify builds from the repository root `netlify.toml`, which sets `website` as
the build base. Connect the repository once and no site settings need to be
entered by hand:

| Setting | Value |
| --- | --- |
| Base directory | `website` |
| Build command | `npm run build` |
| Publish directory | `build` |
| Node version | 20 |

The Netlify site is `chitrplayer.netlify.app`. The Cloudflare Worker that fronts
the short domain proxies `/chitr` to that host; the route lives in the worker's
own repository.

## Downloads

[`src/lib/useRelease.ts`](src/lib/useRelease.ts) reads
`api.github.com/repos/jamal474/chitr/releases/latest` on load and fills in the
version, publication date, and a per-platform asset link and file size. Assets
are matched by looking for `macos`, `windows` or `linux` in the file name, which
is how the release workflow already names them.

If the request fails — rate limit, offline, no published release — every button
falls back to the releases page and the version falls back to
`FALLBACK_VERSION` in [`src/data/site.ts`](src/data/site.ts). Bump that constant
when tagging a release so the page reads correctly before the fetch resolves.

## Content

Page copy lives in [`src/data/content.ts`](src/data/content.ts): feature groups,
the format tables, the keyboard shortcut tables and the roadmap. These duplicate
the repository readme, so update both when the app changes.

## Screenshots

Screenshots are not shipped with the site. They are served from the short
domain's asset route, `https://sabo.sh/storage/project/chitr/<name>.png`, which
the Cloudflare Worker rewrites onto ImageKit — so upload each file to ImageKit
under `project/chitr/`.

`public/screenshots/` is the local staging copy, resized and ignored by git.
The page uses five of them:

| File | Where it appears |
| --- | --- |
| `hero.png` | Hero, under the headline |
| `image-player.png` | Image Player block |
| `image-slideshow.png` | Cross-fades in on hover over the Image Player shot |
| `video-player.png` | Video Player block |
| `video-controls.png` | Full-width band below both players |

Capture at 1600 px wide or more with the application window filling the frame,
and let each image keep its own aspect ratio — the page frames them at whatever
ratio they arrive in. `shot()` in [`src/data/site.ts`](src/data/site.ts) builds
every URL; point `VITE_SCREENSHOT_BASE` at `/chitr/screenshots` to develop
against the local copies instead:

```shell
VITE_SCREENSHOT_BASE=/chitr/screenshots npm run dev
```

`public/demo-thumbnail.png` is the poster frame for the demo video and ships
with the site, copied from `assets/thumbnail.png`.
