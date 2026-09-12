import { useMemo } from 'react';
import ScrollProgress from './components/ScrollProgress';
import SiteHeader from './components/SiteHeader';
import Hero from './components/Hero';
import Features from './components/Features';
import Demo from './components/Demo';
import Reference from './components/Reference';
import Download from './components/Download';
import SiteFooter from './components/SiteFooter';
import { detectPlatform, useRelease } from './lib/useRelease';

export default function App() {
  const release = useRelease();
  const detected = useMemo(detectPlatform, []);

  return (
    <>
      <ScrollProgress />
      <SiteHeader />
      <main>
        <Hero release={release} />
        <Features />
        <Demo />
        <Reference />
        <Download release={release} detected={detected} />
      </main>
      <SiteFooter />
    </>
  );
}
