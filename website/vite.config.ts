import { defineConfig, type Plugin } from 'vite';
import react from '@vitejs/plugin-react';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));

// Served from https://<short-domain>/chitr/, so every emitted asset URL needs
// that prefix. Overridable with BASE_PATH (must start and end with "/").
const base = process.env.BASE_PATH || '/chitr/';

// Output into build/chitr and publish build/ from Netlify, so files sit on
// disk at the same paths they are served from and the proxy never rewrites.
const outDir = path.posix.join('build', base);

// Netlify and the edge router both 301 "/chitr" to "/chitr/"; the dev server
// does not, so it 404s on the bare prefix. This adds the same redirect.
function baseRedirect(): Plugin {
  const bare = base.replace(/\/$/, '');
  return {
    name: 'chitr-base-redirect',
    configureServer(server) {
      server.middlewares.use((req, res, next) => {
        const [path, query] = (req.url || '').split('?');
        if (path !== bare) return next();
        res.writeHead(301, { Location: base + (query ? `?${query}` : '') });
        res.end();
      });
    },
  };
}

export default defineConfig({
  base,
  server: { port: 5174 },
  build: { outDir, emptyOutDir: true },
  resolve: {
    alias: { '@': path.resolve(__dirname, './src') },
  },
  plugins: [react(), baseRedirect()],
});
