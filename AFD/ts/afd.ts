#!/usr/bin/env deno run
type Word = 'a' | 'b';

class State {
    public id: number;
    public onEvent?: (ev: Word) => number;
    public onEnter?: () => void;
    public onExit?: () => void;

    public constructor(id: number, onEvent: (ev) => number, onEnter?: () => void, onExit?: () => void) {
        this.id = id;
        this.onEvent = onEvent;
        this.onEnter = onEnter;
        this.onExit = onExit;
    }
}

class AFD {
    public states: State[];
    public currentState?: State;

    public initialState: number;
    public finalState: number[];

    constructor(initialState: number, finalState: number[]) {
        this.states = [];
        this.currentState = undefined;

        this.initialState = initialState;
        this.finalState = finalState;
    }

    public addState(state: State) {
        this.states.push(state);
    }

    public getState(stateId: number): State | undefined {
        return this.states.find((state) => state.id === stateId);
    }

    public setState(stateId: number) {
        if (this.currentState?.onExit)
            this.currentState.onExit();

        this.currentState = this.getState(stateId);
        if (stateId < 0) {
            this.currentState = undefined;
            return;
        }

        if (this.currentState?.onEnter)
            this.currentState.onEnter();
    }

    public sendEvent(ev: Word) {
        if (!this.currentState) return;

        if (this.currentState.onEvent)
            this.setState(this.currentState.onEvent(ev));
    }

    public validate(word: Word[]): boolean  {
        this.setState(this.initialState);

        for (const ev of word) {
            this.sendEvent(ev);
        }

        if (!this.currentState) return false;

        return this.finalState.includes(this.currentState.id);
    }
}

function setCharStates(args: [string, number][] = [], def: number = -1): (ev) => number {
    return (ev) => {
        console.log(ev);
        for (const [char, state] of args) {
            if (char === ev) return state;
        }
        return def;
    }
}

const stateController = new AFD(0, [99]);

const statePool = [
    new State(0, setCharStates([ ['a', 1] ])),
    new State(1, setCharStates([ ['b', 2], ['a', 99] ])),
    new State(2, setCharStates([ ['b', 1] ])),
    new State(99, () => -1)
]

for (const state of Object.values(statePool)) {
    stateController.addState(state);
}

const validation = stateController.validate(['a', 'b', 'b', 'a']);
console.log(validation);