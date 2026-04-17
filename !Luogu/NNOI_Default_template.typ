#import "@preview/oxifmt:1.0.0": strfmt

#let cjk-align-mark = box(width: 0pt, hide[兔])

#set document(title: "11111")
#set page(paper: "a4", margin: (left: 2.5cm, right: 2.5cm, top: 2.5cm, bottom: 2.5cm))
#set text(lang: "zh", font: ("Latin Modern Roman 12", "Noto Serif CJK SC"), size: 12pt)
#set par(first-line-indent: (amount: 2em, all: true), spacing: 0.7em, leading: 0.7em)

#let in-raw = state("in-raw", false)
#show strong: st => {
  if in-raw.get() { st } else {
    set text(font: ("Latin Modern Roman 12", "SimHei"))
    show regex("\p{sc=Hani}+"): s => {
      underline(s, offset: 3pt, stroke: (
        cap: "round",
        thickness: 0.1em,
        dash: (array: (0em, 1em), phase: 0.5em),
      ))
    }
    st
  }
}
#show heading.where(level: 1): it => {
  set text(size: 18pt, weight: "regular", font: ("Latin Modern Roman 17", "SimHei"))
  set heading(bookmarked: true)
  pad(top: 10pt, align(center, h(2em) + it.body))
}
#show heading.where(level: 2): it => {
  set text(size: 13pt, weight: "regular", font: ("Latin Modern Roman 12", "SimHei"))
  set heading(bookmarked: true)
  pad(left: 1.5em, top: 1em, bottom: .5em, [【] + it.body + [】])
}
#show emph: it => {
  if in-raw.get() { it } else {
    set text(font: "Latin Modern Roman", weight: "bold")
    it
  }
}
#show link: set text(fill: rgb("#ed028c"))
#show raw: it => {
  in-raw.update(true)
  let mono-font = ("Consolas", "SimSun")
  set text(font: mono-font, size: 12pt)
  if not it.block { it } else {
    set par(leading: 0pt, spacing: 0pt)
    set block(above: 10pt, below: 10pt)
    show raw.line: jt => {
      let stroke = (
        left: 0.4pt + rgb("#0000ff"),
        right: 0.4pt + rgb("#0000ff"),
      )
      let inset = (
        top: 8.5pt / 2,
        bottom: 8.5pt / 2,
        left: 3pt,
        right: 3pt,
      )
      if jt.number == 1 {
        stroke.top = 0.4pt + rgb("#0000ff")
        inset.top = 6pt
      }
      if jt.number == jt.count {
        stroke.bottom = 0.4pt + rgb("#0000ff")
        inset.bottom = 9pt
      }
      context (
        box(move(
          dx: 3pt + 6pt,
          box(
            box(
              grid(
                columns: (0pt, 0pt, 100% - 6pt),
                align: (bottom, bottom, bottom),
                move(
                  dx: -9pt - measure([#jt.number]).width,
                  text(fill: rgb("#808080"), size: 10pt, [#jt.number]),
                ),
                cjk-align-mark,
                jt.body,
              ),
            ),
            stroke: stroke,
            inset: inset,
          ),
        ))
      )
    }
    block(it)
  }
  in-raw.update(false)
}
#set raw(tab-size: 4, theme: "./tuackCodeTheme.tmTheme")

#show figure: it => {
  pad(top: 9pt, bottom: 6pt, it)
}
#set figure(numbering: none)

#show math.equation: set text(font: "Latin Modern Math")

#set table(stroke: 0.3pt)

#set table(
  stroke: (x, y) => (
    left: if x > 0 { .4pt },
    bottom: 2pt,
    top: if y == 0 { 2pt } else if y == 1 { 1.2pt } else { .4pt },
  ),
  align: center + horizon,
)

#set heading(depth: 2)

