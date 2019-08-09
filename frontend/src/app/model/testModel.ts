export class TestModel {
  private NAME: string
  private X: number;
  private Y: number;

  get name() {
    return this.NAME;
  }

  set name(value) {
    this.NAME = value;
  }

  get x() {
    return this.X;
  }

  set x(value) {
    this.X = value;
  }

  get y() {
    return this.Y;
  }

  set y(value) {
    this.Y= value;
  }
}
