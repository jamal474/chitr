import type { Config } from 'tailwindcss';

const config: Config = {
  content: ['./index.html', './src/**/*.{ts,tsx}'],
  theme: {
    extend: {
      colors: {
        paper: { DEFAULT: '#f0ede4', soft: '#f7f5ef', deep: '#e6e2d5' },
        ink: { DEFAULT: '#14141a', soft: '#2b2b31', muted: '#5c594e', dim: '#8f8c81' },
        line: { DEFAULT: '#cec9b6', soft: '#e2ddca' },
        accent: {
          DEFAULT: '#c03502',
          bright: '#f44300',
          soft: 'rgba(192, 53, 2, 0.08)',
        },
        signal: { DEFAULT: '#f7d600' },
      },
      fontFamily: {
        serif: ['"Fraunces"', 'Georgia', 'ui-serif', 'serif'],
        mono: ['"JetBrains Mono"', 'ui-monospace', 'SFMono-Regular', 'monospace'],
      },
      fontSize: {
        label: ['0.6875rem', { lineHeight: '1', letterSpacing: '0.14em' }],
        meta: ['0.75rem', { lineHeight: '1.5', letterSpacing: '0.02em' }],
        body: ['1rem', { lineHeight: '1.65' }],
        'display-sm': ['clamp(1.75rem, 3.5vw, 2.5rem)', { lineHeight: '1.02', letterSpacing: '-0.015em' }],
        'display-md': ['clamp(2.25rem, 5vw, 3.75rem)', { lineHeight: '0.98', letterSpacing: '-0.02em' }],
        'display-lg': ['clamp(3rem, 7vw, 5.5rem)', { lineHeight: '0.94', letterSpacing: '-0.02em' }],
      },
      maxWidth: { shell: '1180px', reading: '68ch' },
      spacing: { 'shell-x': '2.5rem' },
      keyframes: {
        'fade-up': {
          from: { opacity: '0', transform: 'translateY(10px)' },
          to: { opacity: '1', transform: 'translateY(0)' },
        },
        'fade-in': { from: { opacity: '0' }, to: { opacity: '1' } },
        'pulse-ring': {
          '0%': { transform: 'scale(1)', opacity: '0.5' },
          '100%': { transform: 'scale(1.7)', opacity: '0' },
        },
      },
      animation: {
        'fade-up': 'fade-up 500ms cubic-bezier(0.2, 0.7, 0.1, 1) both',
        'fade-in': 'fade-in 400ms ease-out both',
        'pulse-ring': 'pulse-ring 1.8s cubic-bezier(0.2, 0.7, 0.1, 1) infinite',
      },
    },
  },
  plugins: [],
};

export default config;
