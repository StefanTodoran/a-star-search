import fse from "fs-extra";
import { OfficialLevelDocument, getAllEntries } from "./database";

function main() {
    // const rawLevels: OfficialLevelDocument[] = await getAllEntries("officialLevels");

    // for (let i = 0; i < rawLevels.length; i++) {
    //     const rawLevel = rawLevels[i];
    //     const existingLevel: OfficialLevel = getData(rawLevel.id);

    //     const updatedLevel: OfficialLevel = {
    //         uuid: rawLevel.id,
    //         name: rawLevel.name,
    //         board: parseCompressedBoardData(rawLevel.board),
    //         completed: existingLevel?.completed,
    //         official: true,
    //         order: rawLevel.order,
    //     };

    //     parsedLevels.push(updatedLevel);
    // }

    // return parsedLevels;
}

main();