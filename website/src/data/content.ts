import type { LucideIcon } from 'lucide-react';
import { Images, Film } from 'lucide-react';

export interface FeatureGroup {
  id: string;
  num: string;
  icon: LucideIcon;
  name: string;
  lede: string;
  shot: string;
  shotAlt: string;
  hoverShot?: string;
  hoverLabel?: string;
  features: { title: string; body: string }[];
}

export const featureGroups: FeatureGroup[] = [
  {
    id: 'image-player',
    num: '01',
    icon: Images,
    name: 'Image Player',
    lede: 'Open one picture and the whole folder comes with it.',
    shot: 'image-player',
    shotAlt: 'Chitr image player showing a picture with the file name in the status bar',
    hoverShot: 'image-slideshow',
    hoverLabel: 'Hover: slideshow running',
    features: [
      { title: 'Multiple formats', body: 'JPG, PNG, GIF, TIFF, PCX, PNM and XPM.' },
      { title: 'Slideshow', body: 'One key runs the folder unattended, the same key stops it.' },
      { title: 'Folder navigation', body: 'Arrow keys step through the folder the file came from.' },
    ],
  },
  {
    id: 'video-player',
    num: '02',
    icon: Film,
    name: 'Video Player',
    lede: 'Platform decoders, controls that answer to the keyboard first.',
    shot: 'video-player',
    shotAlt: 'Chitr video player with the seek bar, timer and volume control visible',
    features: [
      { title: 'Playback controls', body: 'Play, pause and seek from the bar or the keyboard.' },
      { title: 'Interactive seek bar', body: 'Drag to scrub, or press a digit to jump to that tenth.' },
      { title: 'Volume with mute', body: 'One-percent steps on the arrows, one key to silence it.' },
      { title: 'Live timer', body: 'Elapsed and total time, always on screen.' },
    ],
  },
];

export interface FormatTable {
  id: string;
  label: string;
  rows: { platform: string; extensions: string[] }[];
}

export const formatTables: FormatTable[] = [
  {
    id: 'images',
    label: 'Images',
    rows: [
      {
        platform: 'Windows, macOS & Linux',
        extensions: ['JPG', 'PNG', 'GIF', 'TIFF', 'PCX', 'PNM', 'XPM'],
      },
    ],
  },
  {
    id: 'video',
    label: 'Video',
    rows: [
      { platform: 'Windows & macOS', extensions: ['mp4', 'm4v', 'mov', 'mpg'] },
      { platform: 'Windows only', extensions: ['avi', 'mkv', 'wmv'] },
    ],
  },
  {
    id: 'audio',
    label: 'Audio',
    rows: [
      {
        platform: 'Windows & macOS',
        extensions: ['aac', 'aiff', 'au', 'flac', 'm4a', 'mp3', 'wav'],
      },
      { platform: 'Windows only', extensions: ['wma'] },
      { platform: 'macOS only', extensions: ['caf', 'ogg', 'opus'] },
    ],
  },
];

export const formatNote =
  'Format support comes from the media framework each operating system provides. This list is what has been verified on macOS 26 Tahoe and Windows 10.';

export interface ShortcutGroup {
  id: string;
  label: string;
  shortcuts: { keys: string[]; action: string; description: string }[];
}

export const shortcutGroups: ShortcutGroup[] = [
  {
    id: 'image',
    label: 'Image Player',
    shortcuts: [
      { keys: ['←'], action: 'Previous', description: 'Load previous image' },
      { keys: ['→'], action: 'Next', description: 'Load next image' },
      { keys: ['Space'], action: 'Slideshow', description: 'Start or stop the slideshow' },
      { keys: ['O'], action: 'Open', description: 'Select an image file' },
    ],
  },
  {
    id: 'video',
    label: 'Video Player',
    shortcuts: [
      { keys: ['Space', 'K'], action: 'Play / pause', description: 'Toggle playback' },
      { keys: ['←'], action: 'Seek back', description: '−5 seconds' },
      { keys: ['→'], action: 'Seek forward', description: '+5 seconds' },
      { keys: ['J'], action: 'Seek back', description: '−10 seconds' },
      { keys: ['L'], action: 'Seek forward', description: '+10 seconds' },
      { keys: ['0', '–', '9'], action: 'Jump', description: 'Seek to that percentage (3 = 30%)' },
      { keys: ['↑'], action: 'Volume up', description: '+1%' },
      { keys: ['↓'], action: 'Volume down', description: '−1%' },
      { keys: ['M'], action: 'Mute', description: 'Toggle audio' },
      { keys: ['Alt', '+', '←'], action: 'Previous', description: 'Load previous video' },
      { keys: ['Alt', '+', '→'], action: 'Next', description: 'Load next video' },
      { keys: ['O'], action: 'Open', description: 'Select a video file' },
    ],
  },
];

export const roadmap = [
  'Tabs for several players at once',
  'Native file directory integration',
  'Playback status bar with reverse play and rewind',
  'More formats',
];
