import { useRef, type ReactNode } from 'react';
import { motion, useMotionValue, useSpring, useTransform } from 'framer-motion';
import { cn } from '@/lib/utils';

interface TiltFrameProps {
  children: ReactNode;
  className?: string;
  max?: number;
}

/** Tilts its contents a few degrees toward the pointer. */
export default function TiltFrame({ children, className, max = 4 }: TiltFrameProps) {
  const ref = useRef<HTMLDivElement>(null);
  const x = useMotionValue(0);
  const y = useMotionValue(0);

  const spring = { stiffness: 120, damping: 18, mass: 0.4 };
  const rotateX = useSpring(useTransform(y, [-0.5, 0.5], [max, -max]), spring);
  const rotateY = useSpring(useTransform(x, [-0.5, 0.5], [-max, max]), spring);

  function onMove(event: React.MouseEvent<HTMLDivElement>) {
    const rect = ref.current?.getBoundingClientRect();
    if (!rect) return;
    x.set((event.clientX - rect.left) / rect.width - 0.5);
    y.set((event.clientY - rect.top) / rect.height - 0.5);
  }

  function onLeave() {
    x.set(0);
    y.set(0);
  }

  return (
    <div ref={ref} onMouseMove={onMove} onMouseLeave={onLeave} style={{ perspective: 1200 }} className={cn(className)}>
      <motion.div style={{ rotateX, rotateY, transformStyle: 'preserve-3d' }}>{children}</motion.div>
    </div>
  );
}
