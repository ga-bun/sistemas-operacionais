interface Props {
  manual: boolean;
  canNext: boolean;
  onManualChange: (value: boolean) => void;
  onNext: () => void;
}

export default function StepControls({ manual, canNext, onManualChange, onNext }: Props) {
  return (
    <div className="d-flex justify-content-between align-items-center mb-2">
      <div className="form-check form-switch">
        <input
          id="manual-mode"
          className="form-check-input"
          type="checkbox"
          checked={manual}
          onChange={(event) => onManualChange(event.target.checked)}
        />
        <label className="form-check-label" htmlFor="manual-mode">Passo a passo manual</label>
      </div>
      <button type="button" className="btn btn-sm btn-primary" disabled={!manual || !canNext} onClick={onNext}>
        Próximo passo ▶
      </button>
    </div>
  );
}
