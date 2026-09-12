import { shot } from '@/data/site';

interface SwapFrameProps {
  base: string;
  alt: string;
  hover?: string;
  hoverLabel?: string;
}

/** Framed screenshot that cross-fades to a second capture on hover. */
export default function SwapFrame({ base, alt, hover, hoverLabel }: SwapFrameProps) {
  return (
    <figure className="m-0">
      <div className="frame frame-lift group relative">
        <img src={shot(base)} alt={alt} className="block w-full h-auto" />
        {hover && (
          <img
            src={shot(hover)}
            alt=""
            aria-hidden
            className="absolute inset-0 w-full h-full opacity-0 transition-opacity duration-500 group-hover:opacity-100"
          />
        )}
      </div>
      {hoverLabel && <figcaption className="fx-meta text-ink-dim mt-2.5">{hoverLabel}</figcaption>}
    </figure>
  );
}
